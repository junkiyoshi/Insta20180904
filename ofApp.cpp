#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	this->cap.open("video.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_targets = 2;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->targets.push_back(glm::vec4(0.f, 0.f, 0.f, 0.f));
		this->sizes.push_back(glm::vec4(0.f, 0.f, 0.f, 0.f));
	}

	this->face_cascade.load("opencv-3.4.1\\build\\install\\etc\\haarcascades\\haarcascade_frontalface_alt2.xml");
	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->number_of_targets; i++) {

		this->targets[i] = glm::vec4(0.f, 0.f, 0.f, 0.f);
		this->sizes[i] = glm::vec4(0.f, 0.f, 0.f, 0.f);
	}

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(CV_CAP_PROP_POS_FRAMES, 1);
		return;
	}
	else {

		cv::resize(src, this->frame, this->cap_size);
		cv::cvtColor(this->frame, this->frame, CV_BGR2RGB);
		this->image.update();
	}

	cv::Mat gray_frame, small_frame;
	cv::cvtColor(this->frame, gray_frame, CV_RGB2GRAY);
	cv::resize(gray_frame, small_frame, cv::Size(this->cap_size.width * 0.5, this->cap_size.height * 0.5));

	vector<cv::Rect> faces;
	this->face_cascade.detectMultiScale(small_frame, faces);

	int face_count = 0;
	for (cv::Rect r : faces) {

		this->targets[face_count] = glm::vec4(r.x * 2.f + r.width, r.y * 2.f + r.height, 0.f, 0.f);
		float value = r.width < r.height ? r.width : r.height;
		this->sizes[face_count] = glm::vec4(value, value, 0.f, 0.f);
		face_count++;

		if (face_count >= this->number_of_targets) {

			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniformTexture("tex", this->image, 1);
	this->shader.setUniform4fv("targets", &this->targets[0].x, this->number_of_targets);
	this->shader.setUniform4fv("sizes", &this->sizes[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	
	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1280, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}