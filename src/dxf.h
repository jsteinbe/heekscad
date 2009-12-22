// dxf.h
// Copyright (c) 2009, Dan Heeks
// This program is released under the BSD license. See the file COPYING for details.
#pragma once

typedef enum
{
	eUnspecified = 0,	// Unspecified (No units)
	eInches,
	eFeet,
	eMiles,
	eMillimeters,
	eCentimeters,
	eMeters,
	eKilometers,
	eMicroinches,
	eMils,
	eYards,
	eAngstroms,
	eNanometers,
	eMicrons,
	eDecimeters,
	eDekameters,
	eHectometers,
	eGigameters,
	eAstronomicalUnits,
	eLightYears,
	eParsecs
} eDxfUnits_t;


struct SplineData
{
	double norm[3];
	int degree;
	int knots;
	int control_points;
	int fit_points;
	int flag;
	std::list<double> starttanx;
	std::list<double> starttany;
	std::list<double> starttanz;
	std::list<double> endtanx;
	std::list<double> endtany;
	std::list<double> endtanz;
	std::list<double> knot;
	std::list<double> weight;
	std::list<double> controlx;
	std::list<double> controly;
	std::list<double> controlz;
	std::list<double> fitx;
	std::list<double> fity;
	std::list<double> fitz;
};

class CDxfWrite{
private:
	ofstream* m_ofs;
	bool m_fail;

public:
	CDxfWrite(const wxChar* filepath);
	~CDxfWrite();

	bool Failed(){return m_fail;}

	void WriteLine(const double* s, const double* e);
	void WriteArc(const double* s, const double* e, const double* c, bool dir);
    void WriteEllipse(const double* c, double major_radius, double minor_radius, double rotation, double start_angle, double end_angle, bool dir);
	void WriteCircle(const double* c, double radius);
};

// derive a class from this and implement it's virtual functions
class CDxfRead{
private:
	ifstream* m_ifs;

	bool m_fail;
	char m_str[1024];
	eDxfUnits_t m_eUnits;
	wxString m_layer_name;



	bool ReadUnits();
	bool ReadLine();
	bool ReadArc();
	bool ReadCircle();
	bool ReadEllipse();
	bool ReadSpline();
	bool ReadLwPolyLine();
	void OnReadArc(double start_angle, double end_angle, double radius, const double* c);
	void OnReadCircle(const double* c, double radius);
    void OnReadEllipse(const double* c, const double* m, double ratio, double start_angle, double end_angle);
	void OnReadSpline(struct SplineData& sd);
	void get_line();

public:
	CDxfRead(const wxChar* filepath); // this opens the file
	~CDxfRead(); // this closes the file

	bool Failed(){return m_fail;}
	void DoRead(); // this reads the file and calls the following functions

	double mm( const double & value ) const;

	virtual void OnReadLine(const double* s, const double* e){}
	virtual void OnReadArc(const double* s, const double* e, const double* c, bool dir){}
	virtual void OnReadCircle(const double* s, const double* c, bool dir){}
	virtual void OnReadEllipse(const double* c, double major_radius, double minor_radius, double rotation, double start_angle, double end_angle, bool dir){}
	virtual void OnReadSpline(TColgp_Array1OfPnt &control, TColStd_Array1OfReal &weight, TColStd_Array1OfReal &knot,TColStd_Array1OfInteger &mult, int degree, bool periodic, bool rational){}
	virtual void AddGraphics() const { }

    wxString LayerName() const { return(m_layer_name); }

};

class CSketch;

class HeeksDxfRead : public CDxfRead{
private:
    typedef wxString LayerName_t;
	typedef std::map< LayerName_t, CSketch * > Sketches_t;
	Sketches_t m_sketches;

public:
	HeeksDxfRead(const wxChar* filepath):CDxfRead(filepath){}

	static bool m_make_as_sketch;

	// CDxfRead's virtual functions
	void OnReadLine(const double* s, const double* e);
	void OnReadArc(const double* s, const double* e, const double* c, bool dir);
	void OnReadCircle(const double* s, const double* c, bool dir);
    void OnReadEllipse(const double* c, double major_radius, double minor_radius, double rotation, double start_angle, double end_angle, bool dir);
	void OnReadSpline(TColgp_Array1OfPnt &control, TColStd_Array1OfReal &weight, TColStd_Array1OfReal &knot,TColStd_Array1OfInteger &mult, int degree, bool periodic, bool rational);

	void AddObject(HeeksObj *object);
	void AddGraphics() const;
};
