//Sammy Warren and Zach Shikles
// L14_Ex1_Inheritance.cpp
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// --------- BaseSig class and methods ------------------------------
class BaseSig{
	private:
		// neither derived classes nor other users
		// can access private members
	
	protected:	// accessible by derived classes, not by other users.
		int length;
		int *raw_data;
		
	public:
		BaseSig();		// default constructor.
		BaseSig(int L);	// parametric constructor
		~BaseSig();		// destructor
		int getLength() { return length; };
		int getRawValue(int pos);
		static int numObjects;	// static, only one member for the entire hierarchy
		virtual void printInfo();
		void openFile ();
};

void BaseSig::openFile()//contructor to be called if an integer is sent
{
	FILE *fp;
	double maxV;
	fp = fopen("Raw_data_01.txt", "r");//open the file
	fscanf(fp, "%d %lf", &length, &maxV);//assign values to length and max
	//raw_data = new double[length];//allocated memory of proper length

	int i;
	for(i = 0; i < length; i++)
	{
		fscanf(fp, "%d", &raw_data[i]);//load the values into the data variable
	}
	fclose(fp);//close the file
}
int BaseSig::numObjects = 0;	// initialize static data member

// Base class constructor
BaseSig::BaseSig(){
	length = 0;
	raw_data = NULL;
	numObjects++;
	
	
}

// Base class parametric constructor
// Note that the data array is not being initialized (we could read from a file)
BaseSig::BaseSig(int L){
	length = L;
	raw_data = new int[L];
	if(raw_data == NULL)
		cerr << "Error in memory allocation";
	numObjects++;
	openFile();
	
}

// Base class destructor
BaseSig::~BaseSig(){
	delete[] raw_data;
	cout << "Goodbye, BaseSig." << endl;
}

int BaseSig::getRawValue(int pos) {
	if(pos < 0)			// invalid index
		return(raw_data[0]);
	else if(pos >= length)	// invalid index
		return(raw_data[length-1]);	
	else
		return(raw_data[pos]);
}

void BaseSig::printInfo() {
	cout << "\nLength: " << length << endl;
}
// ------------------------------------------------------------------

// --------- ExtendSig class and methods ----------------------------
class ExtendSig : public BaseSig{ // ExtendSig is derived from class BaseSig
//BaseSig is a public base class
	private: 
	protected:
		double average;		// add new data members
		double *data;
		
	public:
		ExtendSig(int L);	//derived classes need a new constructor
		~ExtendSig();
		
		// define new member functions
		double getValue(int pos);
		int setValue(int pos, double val);
		double getAverage();
		
		// redefine member function. Virtual keyword not needed
		void printInfo();	// new standard: explicit "override" keyword can be used
};

// Derived class constructor. Note how the Base constructor is called.
ExtendSig::ExtendSig(int L) : BaseSig(L) {
	data = new double[L];
	if(data == NULL)
		cerr << "Error in memory allocation";
	else{
		for(int i = 0; i < L; i++)
			data[i] = (double)raw_data[i];
		average = getAverage();
	}
}

// Derived class destructor
ExtendSig::~ExtendSig() {
	//delete raw_data;
	delete data;
	cout << "Goodbye, ExtendSig." << endl;
}

double ExtendSig::getValue(int pos) {
	if(pos < 0)			// invalid index
		return(data[0]);
	else if(pos >= length)	// invalid index
		return(data[length-1]);	
	else
		return(data[pos]);
}

int ExtendSig::setValue(int pos, double val) {
	if((pos < 0) || (pos >= length))
		return(-1);	// invalid index
	else {
		data[pos] = val;
		average = getAverage();
		return(0);	// success
	}
}

double ExtendSig::getAverage() {
	if(length == 0)
		return(0.0);
	else {
		double temp = 0.0;
		for(int i = 0; i < length; i++)
			temp += data[i];
		return(temp/(double)length);
	}
}

// Redefined printInfo function for derived class
void ExtendSig::printInfo() {
	cout << "\nLength: " << length << endl
		 << "Average: " << average << endl;
}
// ------------------------------------------------------------------
class ProcessedSig : public BaseSig
{
	private:
		double max;
		double min;
		double mean;
	public:
		double getMax();
		double getMin();
		void normalSig ();
		void printInfo ();//display data length average max min
		void scale(double val);
		void update();
		ProcessedSig (int L);
		void getMean ();
		
};

ProcessedSig:: ProcessedSig (int L): BaseSig(L)
{
	cout << "constructor called"<< endl;
}

void ProcessedSig::update () 
{
	getMax();
	getMin();
	getMean();
	
}
void ProcessedSig::getMean()
{
	if(length == 0)
		mean =(0.0);
	else {
		double temp = 0.0;
		for(int i = 0; i < length; i++)
			temp += raw_data[i];
		mean = (temp/(double)length);
	}
}

double ProcessedSig::getMax()
{
	int i;
	max = raw_data[0];
	for(i = 0; i < length; i++)
	{
		if(raw_data[i] > max)
			max = raw_data[i];
	}
} 
double ProcessedSig::getMin()
{
	int i;
	min = raw_data[0];
	for(i = 0; i < length; i++)
	{
		if(raw_data[i] < min)
			min = raw_data[i];
	}
}
void ProcessedSig:: normalSig ()
{
	getMax();
	scale((1.0/max));
	update ();
}

void ProcessedSig::scale(double val)
{
	int i;
	for(i = 0; i < length; i++)//scales each number of the data array
	{
		raw_data[i] *= val;
	}
	
}
void ProcessedSig::printInfo ()
{
	update();
	cout << "length: " << length << endl
		 << "average:" << mean << endl
		 << "maximum value: " << max << endl
		 << "minimum value: " << min << endl;
}

class ProcessedSig_v2 : ExtendSig
{
	private:
		double max;
		double min;
		double mean;
	public:
		double getMax();
		double getMin();
		void normalSig ();
		void printInfo ();//display data length average max min
		void scale(double val);
		void update();
		ProcessedSig_v2 (int L);
		void getMean ();
	
};

ProcessedSig_v2:: ProcessedSig_v2 (int L): ExtendSig (L){
	cout << "constructor called"<< endl;
}

void ProcessedSig_v2::update () 
{
	getMax();
	getMin();
	average = getAverage();
	
}

double ProcessedSig_v2::getMax()
{
	int i;
	max = raw_data[0];
	for(i = 0; i < length; i++)
	{
		if(raw_data[i] > max)
			max = raw_data[i];
	}
} 
double ProcessedSig_v2::getMin()
{
	int i;
	min = raw_data[0];
	for(i = 0; i < length; i++)
	{
		if(raw_data[i] < min)
			min = raw_data[i];
	}
}
void ProcessedSig_v2:: normalSig ()
{
	getMax();
	scale((1.0/max));
	update ();
}

void ProcessedSig_v2::scale(double val)
{
	int i;
	for(i = 0; i < length; i++)//scales each number of the data array
	{
		raw_data[i] *= val;
	}
	
}
void ProcessedSig_v2::printInfo ()
{
	update();
	cout <<"From ProcessedSig_v2:"<< endl
		 << "length: " << length << endl
		 << "average:" <<average << endl
		 << "maximum value: " << max << endl
		 << "minimum value: " << min << endl;
}
// Main function. A few examples
int main(){
	
	ProcessedSig psig1 (5);
	psig1.printInfo();
	psig1.normalSig();
	psig1.printInfo();
	ProcessedSig_v2 psig2 (8);
	psig2.printInfo();
	return 0;
}