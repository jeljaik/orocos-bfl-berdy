#include "../config.h"
#ifdef __MATRIXWRAPPER_EIGEN__

#include "matrix_EIGEN.h"
#include "vector_EIGEN.h"

#include <Eigen/LU>

using namespace std;

// Passing the constructor arguments...
MyMatrix::Matrix() : EigenMatrix() {}

MyMatrix::Matrix(int num_rows, int num_cols) : EigenMatrix(num_rows, num_cols) {};

//template <typename Derived, int num_rows, int num_cols>
//MyMatrix::Matrix<Derived, num_rows, num_cols>() : Eigen::Matrix<Derived, num_rows, num_cols> {}

// Destructor
MyMatrix::~Matrix(){}

// Copy constructor
MyMatrix::Matrix(const MyMatrix& a) : EigenMatrix(a){}

// ill-designed
MyMatrix::Matrix(const EigenMatrix & a) : EigenMatrix(a){}

MyMatrix::Matrix(int num_rows,const RowVector& v):EigenMatrix(num_rows,v.columns()){
  EigenMatrix & m = *this;
  const EigenRowVector & r = v;
  for(unsigned int i=0;i<num_rows;i++)
    m.row(i) = r;
}

MyRowVector MyMatrix::operator[](unsigned int i) const{
  return this->rowCopy(i);
}

// Size/Capacity
unsigned int MyMatrix::size() const { return this->rows();}
unsigned int MyMatrix::capacity() const { return this->rows();}

// Number of Rows/Cols
unsigned int MyMatrix::rows() const { return ((const EigenMatrix *)this)->rows();}
unsigned int MyMatrix::columns() const { return ((const EigenMatrix *)this)->cols();}

// MATRIX - SCALAR operators
MyMatrix& MyMatrix::operator+= (double a)
{
  EigenMatrix & op1 = *this;
  op1 += EigenMatrix::Constant(op1.rows(), op1.cols(), a);
  return (MyMatrix&)op1;
}

MyMatrix& MyMatrix::operator-= (double a)
{
  EigenMatrix & op1 = (*this);
  op1 -= EigenMatrix::Constant(op1.rows(), op1.cols(), a);
  return (MyMatrix&) op1;
}

MyMatrix& MyMatrix::operator*= (double a)
{
  EigenMatrix & op1 = (*this);
  op1 *= a;
  return *this;
}

MyMatrix& MyMatrix::operator/= (double a)
{
  EigenMatrix & op1 = (*this);
  op1 /= a;
  return (MyMatrix&) op1;
}

MyMatrix MyMatrix::operator+ (double a) const
{
  return (MyMatrix)(((EigenMatrix)(*this)) + EigenMatrix::Constant(rows(), cols(), a));
}

MyMatrix MyMatrix::operator- (double a) const
{
  return (MyMatrix)(((EigenMatrix)(*this)) - EigenMatrix::Constant(rows(), cols(), a));
}

MyMatrix MyMatrix::operator* (double a) const
{
  const EigenMatrix& op1 = (*this);
  return (MyMatrix) (op1 *  a);
}

MyMatrix MyMatrix::operator/ (double a) const
{
  const EigenMatrix& op1 = (*this);
  return (MyMatrix) (op1 /  a);
}

MyMatrix&
MyMatrix::operator =(const MySymmetricMatrix& a)
{
  *this =(MyMatrix) a;

  return *this;
}

// MATRIX - MATRIX Operators
MyMatrix MyMatrix::operator- (const MyMatrix& a) const
{
  const EigenMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix)(op1 - op2);
}

MyMatrix MyMatrix::operator+ (const MyMatrix& a) const
{
  const EigenMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix)(op1 + op2);
}

MyMatrix MyMatrix::operator* (const MyMatrix& a) const
{
  const EigenMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix)(op1 * op2);
}

MyMatrix & MyMatrix::operator+= (const MyMatrix& a)
{
  EigenMatrix & op1 = (*this);
  const EigenMatrix & op2 = a;
  op1 += op2;
  return (MyMatrix &) op1;
}

MyMatrix & MyMatrix::operator-= (const MyMatrix& a)
{
  EigenMatrix & op1 = (*this);
  const EigenMatrix & op2 = a;
  op1 -= op2;
  return (MyMatrix &) op1;
}


// MATRIX - VECTOR Operators
MyColumnVector MyMatrix::operator* (const MyColumnVector &b) const
{
  const EigenMatrix& op1 = (*this);
  return (MyColumnVector) (op1 * ((const EigenColumnVector&)b));
}



double& MyMatrix::operator()(unsigned int a, unsigned int b)
{
  EigenMatrix & op1 = (*this);
  return op1(a-1,b-1);
}

double MyMatrix::operator()(unsigned int a, unsigned int b) const
{
  const EigenMatrix & op1(*this);
  return op1(a-1,b-1);
}

bool MyMatrix::operator==(const MyMatrix& a) const
{
  if (this->rows() != a.rows()) return false;
  if (this->columns() != a.columns()) return false;
  return(((EigenMatrix)(*this)-(EigenMatrix)a).isApproxToConstant(0.0));
}


// Set all elements equal to a
MyMatrix&
 MyMatrix::operator=(double a)
{
  ((EigenMatrix&)(*this)).setConstant(a);
  return *this;
}


MyRowVector MyMatrix::rowCopy(unsigned int r) const
{
  return (MyRowVector) (*this).row(r-1);
}

MyColumnVector MyMatrix::columnCopy(unsigned int c) const
{
  return (MyColumnVector) (*this).col(c-1);
}




MyMatrix MyMatrix::transpose() const
{
  const EigenMatrix &op1 = (*this);
  return (MyMatrix) op1.transpose();
}

double MyMatrix::determinant() const
{
  unsigned int r = this->rows();
  assert(r == this->columns());
  const EigenMatrix& A = (*this);
  return A.determinant();
}


MyMatrix MyMatrix::inverse() const
{
  unsigned int r = this->rows();
  assert(r == this->columns());
  const EigenMatrix& A = (*this);
  return (MyMatrix) A.inverse();
}


int
MyMatrix::convertToSymmetricMatrix(MySymmetricMatrix& sym)
{
    // test if matrix is square matrix
    assert(this->rows() == this->columns() && "[MyMatrix::convertToSymmetricMatrix] The matrix is not squared.");
    
    // NOTE: What I'm tryin to do here is to recognize when a matrix is strictly lower or upper triangular
    // then making it symmetric accordingly.
    
    const EigenMatrix & A = (EigenMatrix &) (*this);
    EigenMatrix tmp(sym.rows(), sym.cols());
    
    // Inspecting upper triangle
    A.triangularView<Eigen::Upper>().evalTo(tmp);
    // Extracting diagonal matrix
    auto diagVec = tmp.diagonal();
    auto diagMatVirtual = Eigen::DiagonalMatrix<double, Eigen::Dynamic>(diagVec);
    EigenMatrix diagMatEigen(tmp.rows(),tmp.cols());
    diagMatVirtual.evalTo(diagMatEigen);
    if ( (tmp - diagMatEigen).sum() == 0 )
    {
        //std::cout << "This matrix is strictly lower triangular. Symmetrizing accordingly" << std::endl;
        sym = MySymmetricMatrix(A.selfadjointView<Eigen::Lower>());
    } else {
        // If it's upper triangular
        A.triangularView<Eigen::Lower>().evalTo(tmp);
        diagVec = tmp.diagonal();
        diagMatVirtual = Eigen::DiagonalMatrix<double, Eigen::Dynamic>(diagVec);
        diagMatVirtual.evalTo(diagMatEigen);
        if ( (tmp - diagMatEigen).sum() == 0 )
        {
            //std::cout << "This matrix is strictly upper triangular. Symmetrizing accordingly"  << std::endl;
            sym = MySymmetricMatrix(A.selfadjointView<Eigen::Upper>());
        } else {
            // A = (A + A^T)/2 mean of both triangular parts
            sym = ( A + A.transpose() )*0.5;
        }
    }
    return 0;
}

void
MyMatrix::resize(unsigned int i, unsigned int j, bool copy, bool initialize)
{
    // By default not a conservative resizing but destructive, unless resizing is equivalent to transposing
  EigenMatrix & temp = (EigenMatrix &) (*this);
    if (copy)
    {
        temp = temp.block(0,0,i,j).eval();
    } else
    {
        temp.resize(i,j);
    }
}

void MyMatrix::setSubMatrix ( const MatrixWrapper::Matrix& b, unsigned int i_start, unsigned int i_end, unsigned int j_start, unsigned int j_end )
{
    EigenMatrix& tmp = (*this);
    int num_rows = (i_end - i_start) + 1;
    int num_cols = (j_end - j_start) + 1;
    tmp.block(i_start-1, j_start-1, num_rows, num_cols) = b;
    // set submatrix from Eigen done here.
}

// get sub matrix
MyMatrix MyMatrix::sub(int i_start, int i_end, int j_start , int j_end) const
{
  const EigenMatrix & A = (EigenMatrix &) (*this);
  MyMatrix submatrix(A.block(i_start-1,j_start-1,i_end-i_start+1,j_end-j_start+1));
  return submatrix;
}

void MyMatrix::setColumn(const MyColumnVector &b, int i) const
{
    EigenMatrix & op = (EigenMatrix &) *(this);
    op.col(i-1) = b.col(0);
}

void MyMatrix::setColumn(MyColumnVector & b, int j)
{
    EigenMatrix & op = (EigenMatrix &) *(this);
    op.col(j-1) = b.col(0);
}

void MyMatrix::eulerToRotation ( double roll, double pitch, double yaw ) {
    EigenMatrix & op = (EigenMatrix &) *(this);
    Eigen::Matrix3d m;
    m = Eigen::AngleAxisd(  roll*(M_PI/180), Eigen::Vector3d::UnitX())
    * Eigen::AngleAxisd( pitch*(M_PI/180), Eigen::Vector3d::UnitY())
    * Eigen::AngleAxisd(   yaw*(M_PI/180), Eigen::Vector3d::UnitZ());
    
    op = m;
//    op(1,1) = m(0); op(1,2) = m(3); op(1,3) = m(6);
//    op(2,1) = m(1); op(2,2) = m(4); op(2,3) = m(7);
//    op(3,1) = m(2); op(3,2) = m(5); op(3,3) = m(8);
    
//    cout << "Rotation matrix for roll: " << roll << " pitch: " << pitch << " yaw: " << endl;
//    cout << m << endl;
    
}

/////////////////////////////
// CLASS SYMMETRIC MATRIX  //
/////////////////////////////

MySymmetricMatrix::SymmetricMatrix() : EigenSymmetricMatrix() {}
MySymmetricMatrix::SymmetricMatrix(int n) : EigenSymmetricMatrix(n,n) {}
MySymmetricMatrix::SymmetricMatrix(int num_rows,const RowVector& v):EigenSymmetricMatrix(num_rows,v.columns()){
  EigenSymmetricMatrix & m = *this;
  const EigenRowVector & r = v;
  for(unsigned int i=0;i<num_rows;i++)
    m.row(i) = r;
}

MyRowVector MySymmetricMatrix::operator[](unsigned int i) const{
  return this->rowCopy(i);
}



// Copy constructor
MySymmetricMatrix::SymmetricMatrix(const SymmetricMatrix& a) : EigenSymmetricMatrix(a){}
MySymmetricMatrix::SymmetricMatrix(const EigenSymmetricMatrix& a) : EigenSymmetricMatrix(a){}
MySymmetricMatrix::SymmetricMatrix(const EigenSymmetricView & a) : EigenSymmetricMatrix(a){}

// Destructor
MySymmetricMatrix::~SymmetricMatrix(){}

// Size/Capacity
unsigned int MySymmetricMatrix::size() const { return this->rows();}
unsigned int MySymmetricMatrix::capacity() const { return this->rows();}

// Ask Number of Rows and Columns
unsigned int MySymmetricMatrix::rows() const { return ((const EigenSymmetricMatrix *)this)->rows();}
unsigned int MySymmetricMatrix::columns() const { return ((const EigenSymmetricMatrix *)this)->cols();}


MyRowVector MySymmetricMatrix::rowCopy(unsigned int r) const
{
  
  unsigned int cols = columns();
  EigenRowVector temp(cols);
  for (unsigned int i=0; i<cols; i++)
    temp(i) = (*this)(r,i+1);
  return (MyRowVector) temp;
}

MySymmetricMatrix MySymmetricMatrix::transpose() const {return (*this);}

MySymmetricMatrix MySymmetricMatrix::inverse() const
{
  unsigned int r = this->rows();
  assert(r == this->columns());
  const EigenSymmetricMatrix& A = (*this);
  // EigenSymmetricView A = ((const EigenSymmetricMatrix *)(this))->selfadjointView<Eigen::Upper>();
  return MySymmetricMatrix(A.inverse());
}

double MySymmetricMatrix::determinant() const
{
  unsigned int r = this->rows();
  assert(r == this->columns());
  const EigenSymmetricMatrix& A = (*this);
  // EigenSymmetricView A = ((const EigenSymmetricMatrix *)(this))->selfadjointView<Eigen::Upper>();
  return A.determinant();
}


// Set all elements equal to a
MySymmetricMatrix& MySymmetricMatrix::operator=(const double a)
{
  ((EigenSymmetricMatrix&)(*this)).setConstant(a);
  return *this;
}


// SYMMETRICMATRIX - SCALAR operators
MySymmetricMatrix& MySymmetricMatrix::operator +=(double a)
{
  EigenSymmetricMatrix & op1 = *this;
  op1 += EigenSymmetricMatrix::Constant(op1.rows(), op1.cols(), a);
  return (MySymmetricMatrix&)op1;
}

MySymmetricMatrix& MySymmetricMatrix::operator -=(double a)
{
  EigenSymmetricMatrix & op1 = *this;
  op1 -= EigenSymmetricMatrix::Constant(op1.rows(), op1.cols(), a);
  return (MySymmetricMatrix&)op1;
}

MySymmetricMatrix& MySymmetricMatrix::operator *=(double b)
{
  EigenSymmetricMatrix & op1 = (*this);
  op1 *= b;
  return (MySymmetricMatrix&) op1;
}

MySymmetricMatrix& MySymmetricMatrix::operator /=(double b)
{
  EigenSymmetricMatrix & op1 = (*this);
  op1 /= b;
  return (MySymmetricMatrix&) op1;
}

MySymmetricMatrix MySymmetricMatrix::operator +(double a) const
{
  return (MySymmetricMatrix)(((EigenSymmetricMatrix)(*this)) + EigenSymmetricMatrix::Constant(rows(), cols(), a));
}

MySymmetricMatrix MySymmetricMatrix::operator -(double a) const
{
  return (MySymmetricMatrix)(((EigenSymmetricMatrix)(*this)) - EigenSymmetricMatrix::Constant(rows(), cols(), a));
}

MySymmetricMatrix MySymmetricMatrix::operator *(double b) const
{
 const EigenSymmetricMatrix& op1 = (*this);
  return (MySymmetricMatrix) (op1 *  b);
}

MySymmetricMatrix MySymmetricMatrix::operator /(double b) const
{
  const EigenSymmetricMatrix& op1 = (*this);
  return (MySymmetricMatrix) (op1 /  b);
}




// SYMMETRICMATRIX - MATRIX operators
MyMatrix& MySymmetricMatrix::operator +=(const MyMatrix& a)
{
  EigenSymmetricMatrix & op1 = (*this);
  op1 += a;
  return (MyMatrix &) op1;
}

MyMatrix& MySymmetricMatrix::operator -=(const MyMatrix& a)
{
  EigenSymmetricMatrix & op1 = (*this);
  op1 -= a;
  return (MyMatrix &) op1;
}


MyMatrix MySymmetricMatrix::operator+ (const MyMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix) (op1 + op2);
}

MyMatrix MySymmetricMatrix::operator- (const MyMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix) (op1 - op2);
}

MyMatrix MySymmetricMatrix::operator* (const MyMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenMatrix& op2 = a;

  return (MyMatrix) (op1 * op2);
}



// SYMMETRICMATRIX - SYMMETRICMATRIX operators
MySymmetricMatrix& MySymmetricMatrix::operator +=(const MySymmetricMatrix& a)
{
  EigenSymmetricMatrix & op1 = (*this);
  const EigenSymmetricMatrix & op2 = a;
  op1 += op2;
  return (MySymmetricMatrix &) op1;
}

MySymmetricMatrix& MySymmetricMatrix::operator -=(const MySymmetricMatrix& a)
{
  EigenSymmetricMatrix & op1 = (*this);
  const EigenSymmetricMatrix & op2 = a;
  op1 -= op2;
  return (MySymmetricMatrix &) op1;
}

MySymmetricMatrix MySymmetricMatrix::operator+ (const MySymmetricMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenSymmetricMatrix& op2 = a;

  return (MySymmetricMatrix) (op1 + op2);
}

MySymmetricMatrix MySymmetricMatrix::operator- (const MySymmetricMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenSymmetricMatrix& op2 = a;

  return (MySymmetricMatrix) (op1 - op2);
}

MyMatrix MySymmetricMatrix::operator* (const MySymmetricMatrix &a) const
{
  const EigenSymmetricMatrix& op1 = *this;
  const EigenSymmetricMatrix& op2 = a;

  return (MyMatrix) (op1 * op2);
}




MyColumnVector MySymmetricMatrix::operator* (const MyColumnVector &b) const
{
  const EigenSymmetricMatrix& op1 = (EigenSymmetricMatrix) *this;
  return (MyColumnVector) (op1 * ((const EigenColumnVector&)b));
}

void MySymmetricMatrix::multiply (const MyColumnVector &b, MyColumnVector &result) const
{
  const EigenSymmetricMatrix& op1 = (EigenSymmetricMatrix) *this;
  result = (MyColumnVector) (op1 * ((const EigenColumnVector&)b));
}

MyMatrix MySymmetricMatrix::sub(int i_start, int i_end, int j_start , int j_end) const
{
  MyMatrix submatrix(i_end-i_start+1, j_end-j_start+1);
  for (int i=i_start; i<=i_end; i++)
    for (int j=j_start; j<=j_end; j++)
      submatrix(i-i_start+1,j-j_start+1) = (*this)(i,j);

  return submatrix;
}

void MySymmetricMatrix::setSubMatrix(const MatrixWrapper::Matrix &b, unsigned int i_start, unsigned int i_end, unsigned int j_start, unsigned int j_end)
{
    
}

double& MySymmetricMatrix::operator()(unsigned int a, unsigned int b)
{
    EigenSymmetricMatrix & op1 = (*this);
    assert(a >= 0 && a < op1.rows()+1);
    assert(b >= 0 && b < op1.cols()+1);
    return op1(a-1,b-1);
}

double MySymmetricMatrix::operator()(unsigned int a, unsigned int b) const
{
    const EigenSymmetricMatrix & op1(*this);
    assert(a >= 0 && a < op1.rows()+1);
    assert(b >= 0 && b < op1.cols()+1);
    return op1(a-1,b-1);
}

bool MySymmetricMatrix::operator==(const MySymmetricMatrix& a) const
{
  if (this->rows() != a.rows()) return false;
  if (this->columns() != a.columns()) return false;
  return(((EigenSymmetricMatrix)(*this)-(EigenSymmetricMatrix)a).isApproxToConstant(0.0));
}

void
MySymmetricMatrix::resize(unsigned int i, bool copy, bool initialize)
{
  EigenSymmetricMatrix & temp = (EigenSymmetricMatrix &) (*this);
  temp.resize(i,i);
}


#endif