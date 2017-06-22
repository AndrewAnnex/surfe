// SURFace Estimator(SURFE) - Terms and Conditions of Use

// Unless otherwise noted, computer program source code of the SURFace Estimator(SURFE)
// is covered under Crown Copyright, Government of Canada, and is distributed under the MIT License.

// The Canada wordmark and related graphics associated with this distribution are protected under 
// trademark law and copyright law.No permission is granted to use them outside the parameters of 
// the Government of Canada's corporate identity program. For more information, see
// http://www.tbs-sct.gc.ca/fip-pcim/index-eng.asp

// Copyright title to all 3rd party software distributed with the SURFace Estimator(SURFE) is held 
// by the respective copyright holders as noted in those files.Users are asked to read the 3rd Party
// Licenses referenced with those assets.

// MIT License

// Copyright(c) 2017 Government of Canada

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files(the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, 
// sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions :

// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef basis_h
#define basis_h

#include <modelling_parameters.h>
#include <modelling_input.h>
#include <gmpxx.h>

#include <Eigen/Core>

using namespace Eigen;

class Kernel{
protected:
	Point *_p1;
	Point *_p2;
public:
	virtual ~Kernel(){}
	void set_points(Point& point1, Point& point2) { _p1 = &point1; _p2 = &point2; }
	void set_center(Point& center) { _p2 = &center; }
	void set_evaluation_points(Point &eval_pt) { _p1 = &eval_pt; }
	Point *p1() const { return _p1; }
	Point *p2() const { return _p2; }
	virtual double basis_pt_pt() = 0;
	virtual double basis_pt_planar_x() = 0;
	virtual double basis_planar_x_pt() = 0;
	virtual double basis_pt_planar_y() = 0;
	virtual double basis_planar_y_pt() = 0;
	virtual double basis_pt_planar_z() = 0;
	virtual double basis_planar_z_pt() = 0;
	virtual double basis_pt_tangent() = 0;
	virtual double basis_tangent_pt() = 0;
	virtual double basis_planar_planar(const Parameter_Types::SecondDerivatives& sd) = 0;
	virtual double basis_tangent_tangent() = 0;
	virtual double basis_planar_tangent(const Parameter_Types::FirstDerivatives& fd ) = 0;
	virtual double basis_tangent_planar(const Parameter_Types::FirstDerivatives& fd ) = 0;
	virtual Kernel *clone() = 0; // convenience function to make a new pointer copy of derived classes
		                            // used for performance in methods->eval_interpolant_at_points(std::vector<Evaluation_Point> &eval_pts)
};

class RBFKernel : public Kernel {
protected:
	double _radius;
	double _x_delta;
	double _y_delta;
	double _z_delta;
	double _c_delta;
	inline void radius();
	inline void scaled_radius();
	// anisotropy members 
	double _Global_Plunge[3];
	Matrix3f _Transform;
	bool get_global_anisotropy(const std::vector<Planar> &planar);
public:
	RBFKernel() : _radius(0), _x_delta(0), _y_delta(0), _z_delta(0), _c_delta(0)
	{
		for (int j = 0; j < 3; j ++ ) _Global_Plunge[j] = NULL;
		_Transform.setZero();
	}
	virtual ~RBFKernel(){}
	virtual double basis() = 0;
	virtual double dx_p1() = 0; // derivative w.r.t. p1's x-coordinate variable
	virtual double dx_p2() = 0; // derivative w.r.t. p2's x-coordinate variable
	virtual double dy_p1() = 0; // derivative w.r.t. p1's y-coordinate variable ...
	virtual double dy_p2() = 0;
	virtual double dz_p1() = 0;
	virtual double dz_p2() = 0;
	virtual double dxx() = 0;
	virtual double dxy() = 0;
	virtual double dxz() = 0;
	virtual double dyx() = 0;
	virtual double dyy() = 0;
	virtual double dyz() = 0;
	virtual double dzx() = 0;
	virtual double dzy() = 0;
	virtual double dzz() = 0;
	double basis_pt_pt();
	double basis_pt_planar_x();
	double basis_planar_x_pt();
	double basis_pt_planar_y();
	double basis_planar_y_pt();
	double basis_pt_planar_z();
	double basis_planar_z_pt();
	double basis_pt_tangent();
	double basis_tangent_pt();
	double basis_planar_planar( const Parameter_Types::SecondDerivatives& sd);
	double basis_tangent_tangent();
	double basis_planar_tangent( const Parameter_Types::FirstDerivatives& fd );
	double basis_tangent_planar( const Parameter_Types::FirstDerivatives& fd );
	virtual RBFKernel *clone() = 0;
};

class Cubic : public RBFKernel {
public:
	~Cubic(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual Cubic *clone() { return new Cubic(*this); }
};

class ACubic : public RBFKernel {
public:
	ACubic(const std::vector<Planar> &planar) { get_global_anisotropy(planar); }
	~ACubic(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual ACubic *clone() { return new ACubic(*this); }
};

class Gaussian : public RBFKernel {
private:
	double _shape_parameter;
public:
	Gaussian(const double& shape_parameter) { _shape_parameter = shape_parameter; }
	~Gaussian(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual Gaussian *clone() { return new Gaussian(*this); }
};

class AGaussian : public RBFKernel {
private:
	double _shape_parameter;
public:
	AGaussian(const double &shape_parameter, const std::vector<Planar> &planar) { _shape_parameter = shape_parameter; get_global_anisotropy(planar); }
	~AGaussian(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual AGaussian *clone() { return new AGaussian(*this); }
};

class MQ : public RBFKernel {
private:
	double _shape_parameter;
public:
	MQ(const double &shape_parameter) { _shape_parameter = shape_parameter; }
	~MQ(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual MQ *clone() { return new MQ(*this); }
};

class MQ3 : public RBFKernel {
private:
	double _c;
public:
	MQ3(const double &shape_parameter) { _c = shape_parameter; }
	~MQ3(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual MQ3 *clone() { return new MQ3(*this); }
};

class AMQ : public RBFKernel {
private:
	double _shape_parameter;
public:
	AMQ(const double &shape_parameter, const std::vector<Planar> &planar) { _shape_parameter = shape_parameter; get_global_anisotropy(planar); }
	~AMQ(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual AMQ *clone() { return new AMQ(*this); }
};

class TPS : public RBFKernel {
public:
	~TPS(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual TPS *clone() { return new TPS(*this); }
};

class ATPS : public RBFKernel {
public:
	ATPS(const std::vector<Planar> &planar) { get_global_anisotropy(planar); }
	~ATPS(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual ATPS *clone() { return new ATPS(*this); }
};

class IMQ : public RBFKernel {
private:
	double _shape_parameter;
public:
	IMQ(const double& shape_parameter) { _shape_parameter = shape_parameter; }
	~IMQ(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual IMQ *clone() { return new IMQ(*this); }
};

class AIMQ : public RBFKernel {
private:
	double _shape_parameter;
public:
	AIMQ(const double &shape_parameter, const std::vector<Planar> &planar) { _shape_parameter = shape_parameter; get_global_anisotropy(planar); }
	~AIMQ(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual AIMQ *clone() { return new AIMQ(*this); }
};

class R : public RBFKernel {
public:
	~R(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual R *clone() { return new R(*this); }
};

class AR : public RBFKernel {
public:
	AR(const std::vector<Planar> &planar) { get_global_anisotropy(planar); }
	~AR(){}
	double basis();
	double dx_p1(); // derivative w.r.t. p1's x-coordinate variable
	double dx_p2(); // derivative w.r.t. p2's x-coordinate variable
	double dy_p1(); // derivative w.r.t. p1's y-coordinate variable ...
	double dy_p2();
	double dz_p1();
	double dz_p2();
	double dxx();
	double dxy();
	double dxz();
	double dyx();
	double dyy();
	double dyz();
	double dzx();
	double dzy();
	double dzz();
	virtual AR *clone() { return new AR(*this); }
};

class Polynomial_Basis {
protected:
	Point *_p;
	bool _truncated;
public:
	Polynomial_Basis() : _truncated(false) { }
	void set_point(Point& point) { _p = &point;}
	virtual VectorXd basis() = 0;
	virtual VectorXd dx() = 0;
	virtual VectorXd dy() = 0;
	virtual VectorXd dz() = 0;
	virtual Polynomial_Basis *clone() = 0;
};

class Poly_Zero : public Polynomial_Basis{
public:
	Poly_Zero(bool truncated = false) { _truncated = truncated; }
	VectorXd basis();
	VectorXd dx();
	VectorXd dy();
	VectorXd dz();
	virtual Poly_Zero *clone() {return new Poly_Zero(*this); }
};

class Poly_First : public Polynomial_Basis {
public:
	Poly_First(bool truncated = false) { _truncated = truncated; }
	VectorXd basis();
	VectorXd dx();
	VectorXd dy();
	VectorXd dz();
	virtual Poly_First *clone() {return new Poly_First(*this); }
};

class Poly_Second : public Polynomial_Basis {
public:
	Poly_Second(bool truncated = false) { _truncated = truncated; }
	VectorXd basis();
	VectorXd dx();
	VectorXd dy();
	VectorXd dz();
	virtual Poly_Second *clone() {return new Poly_Second(*this); }
};

class Lagrangian_Polynomial_Basis {
private:
	Matrix <mpf_class, Dynamic, 1> _polynomial_constants;
	Matrix <mpf_class, Dynamic, Dynamic> _derivative_polynomial_constants;
	bool _get_unisolvent_subset(const std::vector < std::vector < Interface > > &interface_point_lists);
	void _initialize_basis();
public:
	Lagrangian_Polynomial_Basis(const std::vector < std::vector < Interface > > &interface_point_lists)
	{
		mpf_set_default_prec(128.0);
		if (_get_unisolvent_subset(interface_point_lists)) _initialize_basis();
		else
		{
			// throw exception
		}
	}
	Matrix <mpf_class, Dynamic, 1> poly(const Point *p);
	Matrix <mpf_class, Dynamic, 1> poly_dx(const Point *p);
	Matrix <mpf_class, Dynamic, 1> poly_dy(const Point *p);
	Matrix <mpf_class, Dynamic, 1> poly_dz(const Point *p);
	std::vector < Interface > unisolvent_subset_points;
};

class Modified_Kernel : public Kernel {
public:
	Modified_Kernel(RBFKernel* arbfkernel, const std::vector < std::vector < Interface > > &interface_point_lists)
	{ 
		mpf_set_default_prec(128.0);
		_aRBFKernel = arbfkernel;
		_aLPB = new Lagrangian_Polynomial_Basis(interface_point_lists);
	}
	// copy constructor
	Modified_Kernel( const Modified_Kernel& source) : _aRBFKernel(source._aRBFKernel->clone()), _aLPB(source._aLPB){}
	~Modified_Kernel() 
	{ 
		delete this->_aRBFKernel;
	}
	double basis_pt_pt();
	double basis_pt_planar_x();
	double basis_planar_x_pt();
	double basis_pt_planar_y();
	double basis_planar_y_pt();
	double basis_pt_planar_z();
	double basis_planar_z_pt();
	double basis_pt_tangent();
	double basis_tangent_pt();
	double basis_planar_planar( const Parameter_Types::SecondDerivatives& sd);
	double basis_tangent_tangent();
	double basis_planar_tangent(const Parameter_Types::FirstDerivatives& fd );
	double basis_tangent_planar(const Parameter_Types::FirstDerivatives& fd );
	virtual Modified_Kernel *clone() { return new Modified_Kernel(*this); }
private:   
	RBFKernel *_aRBFKernel;
	Lagrangian_Polynomial_Basis *_aLPB;
};

#endif