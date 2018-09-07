#include "shape.hpp"
//Sahil Shah
//sahils2
//subclasses of the Shape class include Rectangle, Circle, Sphere, and RectPrism.
//each subclass has methods to retrieve the parameters of object
//CreateShapes reads the shapes from a file and adds them to a shapeVector
//MaxArea and MaxVolume are aptly named.

//Base class
//Please implement Shape's member functions
//constructor, getName()
//@param name
//Base class' constructor should be called in derived classes'
//constructor to initizlize Shape's private variable
Shape::Shape(string name){
    name_=name;
}
//@return string:name of the shape
string Shape::getName(){
    return name_;
}

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
//@param width:width of the rectangle @param length:length of the rectangle
Rectangle::Rectangle(double width,double length):Shape("Rectangle"){
    width_=width;
    length_=length;
}
//@return double: the area of the rectangle
double Rectangle::getArea() const{
    return width_*length_;//A=W*H
}
//@return double: the volume of the rectangle
double Rectangle::getVolume()const{
    return 0;//V=0
}
//add the lengths and the widths
//@params rec: Rectangle objects
//@return Rectangle: result
Rectangle Rectangle::operator+(const Rectangle& rec){
    double newLength=length_+rec.getLength();
    double newWidth=width_+rec.getWidth();
    Rectangle rect(newWidth, newLength);
    return rect;
}
//subrtract the lengths and the widths
//@params rec: Rectangle objects
//@return Rectangle: result
Rectangle Rectangle::operator-(const Rectangle& rec){
    double newLength= (length_-rec.getLength()>0)?(length_-rec.getLength()):0;
    double newWidth= (width_-rec.getWidth()>0)?(width_-rec.getWidth()):0;
    Rectangle rect(newWidth, newLength);
    return rect;
}

double Rectangle::getWidth()const{return width_;}
double Rectangle::getLength()const{return length_;}


//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
//@param radius: radius of the circle
Circle::Circle(double radius):Shape("Circle"){
    radius_=radius;
}
//@return double: the area of the circle
double Circle::getArea() const{
    return pow(radius_,2)*M_PI;//A=PI*r^2
}
//@return double: volume of the circle
double Circle::getVolume() const{
    return 0;//V=0;
}
//@params cir: Circle object
//@return Circle: result
Circle Circle::operator+(const Circle& cir){
    Circle newCir(radius_+cir.getRadius());
    return newCir;
}
//@params cir: Circle objects
//@return Circle: result
Circle Circle::operator-(const Circle& cir){
    double newRadius=(radius_-cir.getRadius()>0)?(radius_-cir.getRadius()):0;
    Circle newCir(newRadius);
    return newCir;
}

double Circle::getRadius()const{return radius_;}

//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
//@param radius of the sphere
Sphere::Sphere(double radius):Shape("Sphere"){
    radius_=radius;
}
//@return double: the surface area
double Sphere::getArea()const{
    return 4*M_PI*pow(radius_,2);//S=4*PI*r^2
}
//@return double: the volume of the sphere
double Sphere::getVolume()const{
    return (4.0/3.0)*M_PI*pow(radius_,3);//V=(4/3)*PI*R^3
}
//@param Sphere: Sphere object
//@return Sphere: the result
Sphere Sphere::operator+(const Sphere& sph){
    Sphere newSph(radius_+sph.getRadius());
    return newSph;
}
//@param Sphere: Sphere object
//@return Sphere: the result
Sphere Sphere::operator-(const Sphere& sph){
    double newRadius = (radius_-sph.getRadius()>0)?(radius_-sph.getRadius()):0;
    Sphere newSph(newRadius);
    return newSph;
}
double Sphere::getRadius()const{return radius_;}

//Rectprism
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
RectPrism::RectPrism(double width,double length, double height):Shape("RectPrism"){
    width_=width;
    height_=height;
    length_=length;
}
double RectPrism::getArea()const{
    return 2*(length_*width_+length_*height_+height_*width_);
}
double RectPrism::getVolume()const{
    return length_*width_*height_;
}
RectPrism RectPrism::operator+(const RectPrism& rectp){
    RectPrism rp(width_+rectp.getWidth(),length_+rectp.getLength(),height_+rectp.getHeight());
    return rp;

}
RectPrism RectPrism::operator-(const RectPrism& rectp){
    double newWidth=(width_-rectp.getWidth()>0)?(width_-rectp.getWidth()):0;
    double newLength=(length_-rectp.getLength()>0)?(length_-rectp.getLength()):0;
    double newHeight=(height_-rectp.getHeight()>0)?(height_-rectp.getHeight()):0;
    RectPrism rp(newWidth, newLength, newHeight);
    return rp;
}
double RectPrism::getWidth()const{return width_;}
double RectPrism::getHeight()const{return height_;}
double RectPrism::getLength()const{return length_;}

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
	//@@Insert your code here
    vector<Shape*> shapeVector;
    double numShapes;
    string name;
    double radius, length, width, height;
    Shape* myShape;
    ifstream ifs(file_name, ifstream::in);
    ifs>>numShapes;//read the number of shapes from the file
    for(int i=0;i<numShapes;i++){
        ifs>>name;
        if(name=="Rectangle"){
            ifs>>length>>width;
            myShape = new Rectangle(width, length);
        }
        else if(name=="Circle"){
            ifs>>radius;
            myShape = new Circle(radius);
        }
        else if(name=="Sphere"){
            ifs>>radius;
            myShape = new Sphere(radius);
        }
        else if(name=="RectPrism"){
            ifs>>width>>height>>length;
            myShape = new RectPrism(width, length, height);
        }
        shapeVector.push_back(myShape);
    }
	return shapeVector;
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
    for(int i=0;i<shapes.size();i++){
        if(shapes[i]->getArea()>max_area){
            max_area=shapes[i]->getArea();
        }
    }
	return max_area;
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
    for(int i=0;i<shapes.size();i++){
        if(shapes[i]->getVolume()>max_volume){
            max_volume=shapes[i]->getVolume();
        }
    }
	return max_volume;
}
