#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);

	this->box2d.init();
	this->box2d.setGravity(0, 25);
	this->box2d.createBounds();
	this->box2d.setFPS(60);

	this->noise_param = ofRandom(1000);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	if(ofGetFrameNum() % 60 == 45) {
		
		int x_span = 20;
		int height = 60;
		for (int x = x_span * 3; x <= ofGetWidth() - x_span * 3; x += x_span) {

			auto noise_value = ofNoise(x * 0.02, 360, this->noise_param);

			vector<glm::vec2> vertices;
			vertices.push_back(glm::vec2(x - x_span * 0.5, 360));
			vertices.push_back(glm::vec2(x - x_span * 0.5 + x_span * 0.8, 360));
			vertices.push_back(glm::vec2(x - x_span * 0.5 + x_span * 0.8, 360 + ofMap(noise_value, 0, 1, -1, -height)));
			vertices.push_back(glm::vec2(x - x_span * 0.5, 360 + ofMap(noise_value, 0, 1, -1, -height)));


			auto polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[0], vertices[1], vertices[2]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());
			
			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->color_list[(int)ofRandom(this->color_list.size())]);

			polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[0], vertices[2], vertices[3]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());

			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->color_list[(int)ofRandom(this->color_list.size())]);
		}
	}

	for (int i = this->polygon_list.size() - 1; i > -1; i--) {

		this->polygon_life_list[i] -= 1;

		if (this->polygon_life_list[i] < 0) {

			this->polygon_list[i]->destroy();
			this->polygon_list.erase(this->polygon_list.begin() + i);
			this->polygon_life_list.erase(this->polygon_life_list.begin() + i);
			this->polygon_color_list.erase(this->polygon_color_list.begin() + i);
		}
	}

	this->box2d.update();

	if (ofGetFrameNum() % 60 < 45) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 45, 0.1, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofNoFill();
	ofSetColor(39);
	int x_span = 20;
	int height = 60;
	for (int x = x_span * 3; x <= ofGetWidth() - x_span * 3; x += x_span) {

		auto noise_value = ofNoise(x * 0.02, 360, this->noise_param);
		ofDrawRectangle(x - x_span * 0.5, 360, x_span * 0.8, ofMap(noise_value, 0, 1, -1, -height));

	}

	ofFill();
	for (int i = 0; i < this->polygon_list.size(); i++) {

		if (this->polygon_life_list[i] > 64) {

			ofSetColor(this->polygon_color_list[i]);
		}
		else {

			ofSetColor(this->polygon_color_list[i], ofMap(this->polygon_life_list[i], 64, 0, 255, 0));
		}

		this->polygon_list[i]->draw();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}