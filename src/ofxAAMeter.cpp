
#include "ofxAAMeter.h"

ofxAAMeter::ofxAAMeter(string name, int x, int y, int w, int h){
    
    _name = name;
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    _backgroundColor.set(ofColor::black);
    _mainColor.set(ofColor::white);
    
    _minValue = 0.0;
    _maxValue = 1.0;
    
    smoothAmnt = 0.0;
    
    verdana.load("verdana.ttf", 10, true, true);
    verdana.setLineHeight(18.0f);
    verdana.setLetterSpacing(1.037);
    line_h = verdana.getLineHeight();
    
    smoothSlider = new ofxDatGuiSlider("smooth", 0.0, 1.0,smoothAmnt);
    smoothSlider->onSliderEvent(this, &ofxAAMeter::onSliderEvent);
    smoothSlider->setWidth(_w * 1.3, 0.0);
    smoothSlider->setHeight(line_h);
    smoothSlider->setLabelMargin(0.0);
    smoothSlider->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    onOffToggle = new ofxDatGuiToggle("on", TRUE);
    onOffToggle->onButtonEvent(this, &ofxAAMeter::onButtonEvent);
    onOffToggle->setStripeVisible(false);
    onOffToggle->setWidth(_w, 0.1);
    onOffToggle->setHeight(line_h);
    onOffToggle->setLabelMargin(0.0);
    onOffToggle->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    
    _meterOrient = VERTICAL;

}
//------------------------------------------------
void ofxAAMeter::update(){
    //update gui components
    smoothSlider->update();
    onOffToggle->update();
    

}
//------------------------------------------------
void ofxAAMeter::draw(){
    ofPushStyle();
    
    //bounds-box
    ofNoFill();
    ofSetColor(_mainColor);
    ofDrawRectangle(_drawRect);
    
    //valueMeter-------------------------
    
    drawMeter();
    
    //label------------------------------
    drawLabel();

    //SLIDERS

    onOffToggle->draw();
    smoothSlider->drawElemental();
   
    ofPopStyle();
}
//------------------------------------------------
void ofxAAMeter::drawMeter(){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    ofFill();
    float scaledValue = ofMap(_value, _minValue, _maxValue, 0.0, 1.0, true);//clamped value
    float meter_h = -1 * (_h * scaledValue);
    ofDrawRectangle( _w * .25 , _h, _w * .5, meter_h);
    ofPopMatrix();

}
//------------------------------------------------
void ofxAAMeter::drawLabel(){
    
    ofPushMatrix();
    ofTranslate(_x, _y);
    
    //constraing width
    float label_w = verdana.stringWidth(_name);
    if(label_w >= _w){
        float space_ratio = 1 / (label_w / _w);
        verdana.setLetterSpacing(space_ratio);
    }
    //align center
    label_w = verdana.stringWidth(_name);
    int label_x =  _w * .5 - label_w *.5;
    //draw label
    verdana.drawString(_name, label_x , line_h * .75);
    
    ofPopMatrix();

}
//------------------------------------------------
void ofxAAMeter::setPosAndSize(int x, int y, int w, int h){
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    
    _drawRect.set(x, y, w, h);
    
    onOffToggle->setPosition(_x, _y + line_h*1.75);
    smoothSlider->setPosition(_x, _y + line_h*2.75);
    
}
//------------------------------------------------
void ofxAAMeter::setValue(float val){

    float lastVal = _value;
    float newVal = val;
    
    _value = smooth(newVal, lastVal, smoothAmnt);

}//------------------------------------------------
void ofxAAMeter::setYandHeight(int y, int h){
    setPosAndSize(_x, y, _w, h);
}
//------------------------------------------------
void ofxAAMeter::setHeight(float h){
    _h = h;
    _drawRect.setHeight(h);
}
//------------------------------------------------
void ofxAAMeter::onSliderEvent(ofxDatGuiSliderEvent e){
    cout << _name <<"::slider: " <<e.value << endl;
    smoothAmnt = e.value;
}
//------------------------------------------------

void ofxAAMeter::onButtonEvent(ofxDatGuiButtonEvent e){
   cout << _name << "- on-off: "<<e.enabled << endl;
}
//------------------------------------------------
float ofxAAMeter::smooth(float newValue,  float lastValue, float amnt){
    
    float result = lastValue * amnt + (1-amnt) * newValue;
    return result;
}


