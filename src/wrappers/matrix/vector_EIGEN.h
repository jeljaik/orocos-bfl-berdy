#include "../config.h"
#ifdef __MATRIXWRAPPER_EIGEN__

#ifndef __VECTOR_EIGEN__
#define __VECTOR_EIGEN__

#include "matrix_wrapper.h"
#include "vector_wrapper.h"
#include "quaternion_wrapper.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

typedef Eigen::VectorXd EigenColumnVector;
typedef Eigen::RowVectorXd EigenRowVector;
typedef Eigen::Quaterniond EigenQuaternion;


namespace MatrixWrapper
{

/// Wrapper class for ColumnVectors (Eigen implementation)
class ColumnVector : public EigenColumnVector, public ColumnVector_Wrapper
{
public:

  /// Constructor
  ColumnVector();

  /// Constructor
  ColumnVector(int nrows);
  ColumnVector(int nrows,double value);

  /// Constructor
  ColumnVector(const MyColumnVector& a, const MyColumnVector& b);

  /// Constructor from quaternion
    ColumnVector(MyQuaternion& quat);
  /// Constructor from pointer to double
  ColumnVector(double* yvec, unsigned int size);
    
  /// Destructor
  virtual ~ColumnVector();

  /// Copy constructor
  ColumnVector (const MyColumnVector& a);

  /// Copy constructor
  ColumnVector (const EigenColumnVector& a);

  virtual void resize(int num_rows);
  virtual unsigned int rows() const;
  virtual unsigned int columns() const;
  virtual unsigned int capacity() const;
  virtual void assign(int size, double value) ;
  virtual ColumnVector vectorAdd(const MyColumnVector& v2) const;

  using EigenColumnVector::operator ==;
  using EigenColumnVector::operator =;
  using EigenColumnVector::operator +=;
  using EigenColumnVector::operator -=;
  using EigenColumnVector::operator +;
  using EigenColumnVector::operator -;

  virtual ColumnVector& operator =(const MyColumnVector& a);
  virtual ColumnVector& operator =(double a);

  virtual MyColumnVector & operator+= (const MyColumnVector& a);
  virtual MyColumnVector & operator-= (const MyColumnVector& a);
  virtual MyColumnVector operator+ (const MyColumnVector &a) const;
  virtual MyColumnVector operator- (const MyColumnVector &a) const;

  virtual MyColumnVector& operator+= (double b);
  virtual MyColumnVector& operator-= (double b);
  virtual MyColumnVector& operator*= (double b);
  virtual MyColumnVector& operator/= (double b);
  virtual MyColumnVector operator+ (double b) const;
  virtual MyColumnVector operator- (double b) const;
  virtual MyColumnVector operator* (double b) const;
  virtual MyColumnVector operator/ (double b) const;

  /// element indexing STARTING FROM 0
  virtual double operator[](unsigned int i) const
  { 
  //std::cout << "(BOOSTVECTOR) operator[] called " << i << std::endl;
   // if (i==0)
   //     std::cout << "(BOOSTVECTOR) operator[0]" << std::endl;
    
   return (*this)(i+1);
  }

  /// element indexing STARTING FROM 0
  virtual double& operator[](unsigned int i) 
  { 
  //std::cout << "(BOOSTVECTOR) operator[] called " << i << std::endl;
  //  if (i==0)
  //      std::cout << "(BOOSTVECTOR) operator[0]" << std::endl;
     return (*this)(i+1);
  }

  virtual double operator()(unsigned int) const;
  virtual bool operator==(const MyColumnVector& a) const;
  virtual double& operator()(unsigned int);
  virtual MyMatrix operator* (const MyRowVector &a) const;
  virtual MyColumnVector sub(int j_start , int j_end) const;
  virtual MyRowVector transpose() const;


};

/// Wrapper class for RowVectors (Eigen implementation)
class RowVector : public EigenRowVector, public RowVector_Wrapper
{
  // No private member:  We don't add anything.

  // Public Members
 public:
  RowVector();
  RowVector(int ncols);
  RowVector(int ncols,double value);
  // If you have another constructor in the matrix library you
  // want to use, you'll have to redefine it yourself

  // Copy constructor
  RowVector (const MyRowVector& a);
  // Copy constructor for Eigen
  RowVector (const EigenRowVector& a);

  virtual ~RowVector();

  virtual void resize(int num_cols);
  virtual void assign(int size, double value) ;
  virtual RowVector vectorAdd(const MyRowVector& v2) const;
  virtual unsigned int rows() const;
  virtual unsigned int columns() const;
  virtual unsigned int capacity() const;
  virtual RowVector& operator =(double a);
  virtual RowVector& operator =(const MyRowVector& a);

  using EigenRowVector::operator ==;
  using EigenRowVector::operator =;
  using EigenRowVector::operator +=;
  using EigenRowVector::operator -=;
  using EigenRowVector::operator +;
  using EigenRowVector::operator -;

  virtual MyRowVector & operator+= (const MyRowVector& a);
  virtual MyRowVector & operator-= (const MyRowVector& a);
  virtual MyRowVector operator+ (const MyRowVector &a) const;
  virtual MyRowVector operator- (const MyRowVector &a) const;

  virtual MyRowVector& operator+= (double b);
  virtual MyRowVector& operator-= (double b);
  virtual MyRowVector& operator*= (double b);
  virtual MyRowVector& operator/= (double b);
  virtual MyRowVector operator+ (double b) const;
  virtual MyRowVector operator- (double b) const;
  virtual MyRowVector operator* (double b) const;
  virtual MyRowVector operator/ (double b) const;

  virtual double operator()(unsigned int) const;
  virtual bool operator==(const MyRowVector& a) const;
  virtual double& operator()(unsigned int);
  virtual MyRowVector sub(int j_start , int j_end) const;
  virtual MyColumnVector transpose() const;
  virtual double operator*(const MyColumnVector& a) const;

};
    
/// Wrapper class for Quaternion (Eigen implementation)
class Quaternion : public EigenQuaternion, public Quaternion_Wrapper
{
public:
    // Constructors
    Quaternion();
    Quaternion(double real, double i, double j, double z);
    Quaternion(MyColumnVector);
    Quaternion(const MyQuaternion& q);
    // Copy constructor for boost
    Quaternion(const EigenQuaternion& q);
    
    // Destructor
    virtual ~Quaternion();
    
    /// Operators
    virtual double& operator()(unsigned int i);
    virtual double operator()(unsigned int i) const;
    virtual MyQuaternion& operator=(const MyQuaternion &q);
    virtual MyQuaternion operator+ (const MyQuaternion &q) const;
    virtual MyQuaternion operator- (const MyQuaternion &q) const;
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat);
    std::string toString() const;
    Quaternion normalize();
    virtual bool conjugate(MyQuaternion& output);
};
    
}

#endif

#endif