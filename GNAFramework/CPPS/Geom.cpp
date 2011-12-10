#include "../Geom.h"
#include "../MathHelper.h"
#include "../GNAExeptions.h"
#include <stdlib.h>

using namespace GNAFramework;

// <editor-fold defaultstate="collapsed" desc="Rectangle:">

const RectangleF RectangleF::Empty(0.f, 0.f, 0.f, 0.f);


void RectangleF::Intersect(const RectangleF *value1, const RectangleF *value2, RectangleF *result){
    result->Y = MathHelper::Max(value1->Y, value2->Y);
    result->height = MathHelper::Min(value1->Bottom(), value2->Bottom()) - result->Y;
    if(result->height < 0){
        *result = Empty;
        return;
    }
    
    result->X = MathHelper::Max(value1->X, value2->X);
    result->width = MathHelper::Min(value1->Right(), value2->Right()) - result->X;
    if(result->width < 0){
        *result = Empty;
        return;
    }
}

RectangleF RectangleF::Intersect(RectangleF value1, RectangleF value2){
    RectangleF result;
    Intersect(&value1, &value2, &result);
    return result;
}

void RectangleF::Union(const RectangleF *value1, const RectangleF *value2, RectangleF *result){
    result->Y = MathHelper::Min(value1->Y, value2->Y);
    result->height = MathHelper::Max(value1->Bottom(), value2->Bottom()) - result->Y;
    result->X = MathHelper::Min(value1->X, value2->X);
    result->width = MathHelper::Max(value1->Right(), value2->Right()) - result->X;
}

RectangleF RectangleF::Union(RectangleF value1, RectangleF value2){
    RectangleF result;
    Union(&value1, &value2, &result);
    return result;
}


bool RectangleF::operator ==(const RectangleF& other) const{
    return (this->X == other.X) &&
                (this->Y == other.Y) &&
                    (this->height == other.height) &&
                        (this->width == other.width);
}

bool RectangleF::operator !=(const RectangleF& other) const{
    return (this->X != other.X) ||
                (this->Y != other.Y) ||
                    (this->height != other.height) ||
                        (this->width != other.width);
}


RectangleF::RectangleF() {
    this->X = 0.f;
    this->Y = 0.f;
    this->width = 0.f;
    this->height = 0.f;
}

RectangleF::RectangleF(float x, float y, float width, float height) {
    setValue(x, y, width, height);
}

void RectangleF::setValue(float x, float y, float width, float height) {
    this->X = x;
    this->Y = y;
    this->width = width;
    this->height = height;
}


Vector2 RectangleF::Center() const{
    Vector2 result(X + width/2, Y + height/2);
    return result;
}


bool RectangleF::IsEmpty() const{
    return (X==0) && (Y==0) && (width == 0) && (height == 0);
}

Vector2 RectangleF::getLocation() const{
    Vector2 result(X, Y);
    return result;
}
void RectangleF::setLocation(Vector2 value){
    X = value.X;
    Y = value.Y;
}

bool RectangleF::Contains(float x, float y) const{
    return (x >= X && x <= X + width && y >= Y && y <= Y + height);
}

void RectangleF::Contains(const Vector2 *value, bool *result) const{
    *result = Contains(value->X, value->Y);
}

bool RectangleF::Contains(Vector2 value) const{
    bool result;
    Contains((const Vector2 *) &value, (bool *) &result);
    return result;
}

void RectangleF::Contains(const RectangleF *value, bool *result) const{
    *result =   (Left() <= value->Left()) &&
                    (Right() >= value->Right()) &&
                        (Bottom() >= value->Bottom()) &&
                            (Top() <= value->Top());
}

bool RectangleF::Contains(RectangleF value) const{
    bool result;
    Contains(&value, &result);
    return result;
}

void RectangleF::Inflate(int horizontalAmount, int verticalAmount){
    X -= horizontalAmount;
    Y -= verticalAmount;
    width += 2 * horizontalAmount;
    height += 2 * verticalAmount;
}

void RectangleF::Intersects(const RectangleF *value, bool *result) const{
    if(value->Right() < Left() || value->Left() > Right())
        *result = false;
    else if(value->Top() < Bottom() || value->Bottom() > Top())
        *result = false;
    else
        *result = true;
}

bool RectangleF::Intersects(RectangleF value) const{
    bool result;
    Intersects(&value, &result);
    return result;
}

void RectangleF::Offset(int offsetX, int offsetY){
    X += offsetX;
    Y += offsetY;
}

void RectangleF::Offset(Vector2 amount){
    Offset(amount.X, amount.Y);
}


//Rectangle::Rectangle(const Rectangle& orig) {}

RectangleF::~RectangleF() {
}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Circle:">

const Circle Circle::Empty(0.f, 0.f, 0.f);

const Circle Circle::Unitary(0.f, 0.f, 1.f);


bool Circle::operator ==(const Circle& other) const{
    return (this->X == other.X) &&
                (this->Y == other.Y) &&
                    (this->radio == other.radio);
}

bool Circle::operator !=(const Circle& other) const{
    return (this->X != other.X) ||
                (this->Y != other.Y) ||
                    (this->radio != other.radio);
}

float Circle::distance(float x, float y) const{
    float dx = X - x, dy = Y - y;
    return sqrt(dx*dx + dy*dy);
}

float Circle::distance(Vector2 vec) const{
    return distance(vec.X, vec.Y);
}

Circle::Circle() {
    this->X = 0.f;
    this->Y = 0.f;
    this->radio = 0.f;
}

Circle::Circle(float x, float y, float radio) {
    setValue(x, y, radio);
}

void Circle::setValue(float x, float y, float radio) {
    this->X = x;
    this->Y = y;
    this->radio = radio;
}

Vector2 Circle::Center() const{
    Vector2 result(X, Y);
    return result;
}


bool Circle::IsEmpty() const{
    return (X==0) && (Y==0) && (radio == 0);
}

Vector2 Circle::getLocation() const{
    Vector2 result(X, Y);
    return result;
}
void Circle::setLocation(Vector2 value){
    X = value.X;
    Y = value.Y;
}

void Circle::toRectangle(RectangleF *rec) const {
    rec->setValue(X - radio, Y - radio, 2*radio, 2*radio);
}

RectangleF Circle::toRectangle() const {
    RectangleF rec;
    toRectangle(&rec);
    return rec;
}

bool Circle::Contains(float x, float y) const{
    return distance(x, y) <= radio;
}

void Circle::Contains(const Vector2 *value, bool *result) const{
    *result = Contains(value->X, value->Y);
}

bool Circle::Contains(Vector2 value) const{
    bool result;
    Contains(&value, &result);
    return result;
}

void Circle::Inflate(int Amount){
    radio += Amount;
}

void Circle::Intersects(const Circle *value, bool *result) const{
    *result = distance(value->X, value->Y) <= radio + value->radio;
}

bool Circle::Intersects(Circle value) const{
    bool result;
    Intersects(&value, &result);
    return result;
}

#include <stdio.h>

void Circle::Intersects(const RectangleF *value, bool *result) const{
    if (value->Contains(Center()))
    {
        *result = true;
        return;
    }
    
    int i;
    Vector2 esquinas[4] = {
        Vector2(value->Left() , value->Top()   ),
        Vector2(value->Right(), value->Top()   ),
        Vector2(value->Left() , value->Bottom()),
        Vector2(value->Right(), value->Bottom())
    };

    for(i = 0; i < 4; i++)
    {
        if (Contains(esquinas[i]))
        {
            *result = true;
            return;
        }
    }
    
    //Bordes laterales:
    if (value->Top() <= Y && value->Bottom() >= Y)
    {
        if (abs(value->Left() - X) <= radio || abs(value->Right() - X) <= radio)
        {
            *result = true;
            return;
        }
    }

    //Bordes Superior e Inferior:
    if (value->Left() <= X && value->Right() >= X)
    {
        if (abs(value->Bottom() - Y) <= radio || abs(value->Top() - Y) <= radio)
        {
            *result = true;
            return;
        }
    }

    *result = false;
}

bool Circle::Intersects(RectangleF value) const{
    bool result;
    Intersects(&value, &result);
    return result;
}

void Circle::Offset(int offsetX, int offsetY){
    X += offsetX;
    Y += offsetY;
}

void Circle::Offset(Vector2 amount){
    Offset(amount.X, amount.Y);
}


//Rectangle::Rectangle(const Rectangle& orig) {}

Circle::~Circle() {
}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Vector2:">

const Vector2 Vector2::Zero(0.f, 0.f);
const Vector2 Vector2::UnitX(1.f, 0.f);
const Vector2 Vector2::UnitY(0.f, 1.f);
const Vector2 Vector2::One(1.f, 1.f);

float Vector2::Dot(Vector2 value1, Vector2 value2){
    float result;
    Vector2::Dot(&value1, &value2, &result);
    return result;
}

Vector2::Vector2() {
    setVal(0.f, 0.f);
}

Vector2::Vector2(float X, float Y) {
    setVal(X, Y);
}

void Vector2::setVal(float X, float Y) {
    this->X = X;
    this->Y = Y;
}

void Vector2::rotate(Vector2 Center, float ang) {
    float xtemp;
    X = X - Center.X;
    Y = Y - Center.Y;

    xtemp = X * cos(ang) - Y * sin(ang);
    Y = X * sin(ang) + Y * cos(ang);
    X = xtemp;

    X += Center.X;
    Y += Center.Y;
}// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Vector3:">
const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
const Vector3 Vector3::One (1.f, 1.f, 1.f);

const Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
const Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
const Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);

const Vector3 Vector3::Backward( 0.f,  0.f,  1.f);
const Vector3 Vector3::Down    ( 0.f, -1.f,  0.f);
const Vector3 Vector3::Forward ( 0.f,  0.f, -1.f);
const Vector3 Vector3::Left    (-1.f,  0.f,  0.f);
const Vector3 Vector3::Right   ( 1.f,  0.f,  0.f);
const Vector3 Vector3::Up      ( 0.f,  1.f,  0.f);

void  Vector3::Cross(const Vector3* value1, const Vector3* value2, Vector3 *result) {
    result->X = value1->Y * value2->Z - value1->Z * value2->Y;
    result->Y = value1->Z * value2->X - value1->X * value2->Z;
    result->Z = value1->X * value2->Y - value1->Y * value2->X;
}


Vector3 Vector3::Cross(Vector3 value1, Vector3 value2) {
    Vector3 r;
    Cross(&value1, &value2, &r);
    return r;
}


Vector3::Vector3() {
    this->X = 0.f;
    this->Y = 0.f;
    this->Z = 0.f;
}

Vector3::Vector3(float X, float Y, float Z) {
    setVal(X, Y, Z);
}

void Vector3::setVal(float X, float Y, float Z) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
}


// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Vector4:">

Vector4::Vector4() {
    this->X = 0.f;
    this->Y = 0.f;
    this->Z = 0.f;
    this->W = 0.f;
}

Vector4::Vector4(float X, float Y, float Z, float W) {
    setVal(X, Y, Z, W);
}

void Vector4::setVal(float X, float Y, float Z, float W) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
    this->W = W;
}
// </editor-fold>
 

// <editor-fold defaultstate="collapsed" desc="Matrix:">

const float  Matrix::iValues[16] = {1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,  0.f, 0.f, 1.f, 0.f,  0.f, 0.f, 0.f, 1.f};
const Matrix Matrix::Identity(iValues);

const float  Matrix::onesValues[16] = {1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f};
const Matrix Matrix::Ones(onesValues);

const float  Matrix::zerosValues[16] = {0.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f, 0.f};
const Matrix Matrix::Zeros(zerosValues);

Matrix Matrix::Lerp(Matrix *matrix1, Matrix *matrix2, float amount){
    Matrix r;
    for(int i = 0; i < 16; i++)
        r.cells[i] = (1.f - amount)*matrix1->cells[i] + amount*matrix2->cells[i];
    return r;
}

Matrix Matrix::CreateLookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector){
    Vector3 zAxis = Vector3::Normalize(cameraTarget - cameraPosition);
    Vector3 xAxis = Vector3::Normalize(Vector3::Cross(zAxis, cameraUpVector));
    Vector3 yAxis = Vector3::Cross(xAxis, zAxis);
    Vector3 m_cam = cameraPosition * (-1.f);
    Vector3 dot(
                Vector3::Dot(xAxis, m_cam),
                Vector3::Dot(yAxis, m_cam),
                Vector3::Dot(zAxis, m_cam)
            );
    
    Matrix r;
    r.cells[ 0] =  xAxis.X; r.cells[ 1] =  yAxis.X; r.cells[ 2] = -zAxis.X; r.cells[ 3] = 0.f;
    r.cells[ 4] =  xAxis.Y; r.cells[ 5] =  yAxis.Y; r.cells[ 6] = -zAxis.Y; r.cells[ 7] = 0.f;
    r.cells[ 8] =  xAxis.Z; r.cells[ 9] =  yAxis.Z; r.cells[10] = -zAxis.Z; r.cells[11] = 0.f;
    r.cells[12] =    dot.X; r.cells[13] =    dot.Y; r.cells[14] =   -dot.Z; r.cells[15] = 1.f;
    
    return r;
}

Matrix Matrix::CreatePerspective(
                float width,
                float height,
                float nearPlaneDistance,
                float farPlaneDistance
        ) {
    
    float r = width/2.f;
    float t = height/2.f;
    float n = nearPlaneDistance;
    float f = farPlaneDistance;
    
    Matrix m = Matrix();
    
    m.cells[ 0] = n/r;
    m.cells[ 5] = n/t;
    m.cells[10] = -(f+n) / (f-n);
    m.cells[14] = -2*f*n / (f-n);
    m.cells[11] = -1;
    
    return m;
}
        
Matrix Matrix::CreatePerspective(Frustum frustum) {
    return CreatePerspectiveFieldOfView(
                frustum.getFovY(), 
                frustum.getAspectRatio(),
                frustum.getFront(),
                frustum.getBack()
            );
}

Matrix Matrix::CreatePerspectiveFieldOfView(
        float fieldOfView,
        float aspectRatio,
        float nearPlaneDistance,
        float farPlaneDistance
) {
    const float DEG2RAD = Pi / 180.f;

    float tangent = tan(fieldOfView / 2 * DEG2RAD);
    float height  = nearPlaneDistance * tangent;
    float width   = height * aspectRatio;
    
    return CreatePerspective(2 * width, 2 * height, nearPlaneDistance, farPlaneDistance);;
}


Matrix Matrix::CreateWorld(
                Vector3 position,
                Vector3 forward,
                Vector3 up
){
    Matrix m = Matrix::Identity;
    m.setUp(up);
    m.setForward(forward);
    m.setRight(Vector3::Cross(up, forward));
    m.setTranslation(position);
    return m;
}



Matrix Matrix::CreateScale(float scale) {
    Matrix r;  CreateScale(scale, &r); return r;
}
void   Matrix::CreateScale(float scale, Matrix *result) {
    CreateScale(scale, scale, scale, result);
}
Matrix Matrix::CreateScale(Vector3 scale) {
    Matrix r;  CreateScale(scale, &r); return r;
}
void   Matrix::CreateScale(Vector3 scale, Matrix *result){
    CreateScale(scale.X, scale.Y, scale.Z, result);
}
Matrix Matrix::CreateScale(float scaleX, float scaleY, float scaleZ) {
    Matrix r;  CreateScale(scaleX, scaleY, scaleZ, &r); return r;
}
void   Matrix::CreateScale(float scaleX, float scaleY, float scaleZ, Matrix *result) {
    *result = Matrix::Identity;
    result->cells[ 0] = scaleX;
    result->cells[ 5] = scaleY;
    result->cells[10] = scaleZ;
}


Matrix Matrix::CreateTranslation(Vector3 translation) {
    Matrix r;  CreateTranslation(translation, &r); return r;
}
void Matrix::CreateTranslation(Vector3 translation, Matrix *result){
    CreateTranslation(translation.X, translation.Y, translation.Z, result);
}
Matrix Matrix::CreateTranslation(float translationX, float translationY, float translationZ){
    Matrix r;  CreateTranslation(translationX, translationY, translationZ, &r); return r;
}
void Matrix::CreateTranslation(float translationX, float translationY, float translationZ, Matrix *result){
    *result = Matrix::Identity;
    result->cells[12] = translationX;
    result->cells[13] = translationY;
    result->cells[14] = translationZ;
}




void   Matrix::CreateRotationX(float radians, Matrix *result) {
    *result = Matrix::Identity;
    result->M22(cos(radians));
    result->M23(sin(radians));
    result->M32(-sin(radians));
    result->M33(cos(radians));
}

Matrix Matrix::CreateRotationX(float radians){
    Matrix result;
    CreateRotationX(radians, &result);
    return result;
}

void   Matrix::CreateRotationY(float radians, Matrix *result) {
    *result = Matrix::Identity;
    result->M11(cos(radians));
    result->M13(-sin(radians));
    result->M31(sin(radians));
    result->M33(cos(radians));
}

Matrix Matrix::CreateRotationY(float radians){
    Matrix result;
    CreateRotationY(radians, &result);
    return result;
}

void   Matrix::CreateRotationZ(float radians, Matrix *result) {
    *result = Matrix::Identity;
    result->M11(cos(radians));
    result->M12(sin(radians));
    result->M21(-sin(radians));
    result->M22(cos(radians));
}

Matrix Matrix::CreateRotationZ(float radians){
    Matrix result;
    CreateRotationZ(radians, &result);
    return result;
}




Matrix Matrix::Invert() {
    Matrix r; return r;
}


Matrix::Matrix(){
    for(int i = 0; i < 16; i++){
        cells[i] = 0.f;
    }
}

Matrix::Matrix(const float values[16]){
    for(int i = 0; i < 16; i++){
        cells[i] = values[i];
    }
}


bool Matrix::operator==(const Matrix & other) const {
    for(int i = 0; i < 16; i++)
        if(cells[i] != other.cells[i]) return false;
    return true;
}
bool Matrix::operator!=(const Matrix & other) const {
    for(int i = 0; i < 16; i++)
        if(cells[i] != other.cells[i]) return true;
    return false;
}

Matrix Matrix::operator*(const Matrix & other) const{
    Matrix r;
    float *p;
    int i, j, k;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            p = r.M_ptr(i, j);
            *p = 0.f;
            for(k = 0; k < 4; k++)
                *p += M(k,j) * other.M(i,k);
        }
    }
    return r;
}

Vector4 Matrix::operator*(const Vector4 &vec) const{
    Vector4 r(
        vec.X * M11() +  vec.Y * M12() + vec.Z * M13()+ vec.W * M14(), //X
        vec.X * M21() +  vec.Y * M22() + vec.Z * M23()+ vec.W * M24(), //Y
        vec.X * M31() +  vec.Y * M32() + vec.Z * M33()+ vec.W * M34(), //Z
        vec.X * M41() +  vec.Y * M42() + vec.Z * M43()+ vec.W * M44()  //W
    );
    return r;
}

Vector3 Matrix::operator*(const Vector3 &vec) const{
    Vector3 r(
        vec.X * M11() +  vec.Y * M12() + vec.Z * M13() + M14(), //X
        vec.X * M21() +  vec.Y * M22() + vec.Z * M23() + M24(), //Y
        vec.X * M31() +  vec.Y * M32() + vec.Z * M33() + M34() //Z
    );
    return r;
}

Matrix Matrix::operator*(const float  & scalar) const{
    Matrix r;
    for(int i = 0; i < 16; i++)
        r.cells[i] = cells[i] * scalar;
    return r;
}

Matrix Matrix::operator+(const Matrix & other) const {
    Matrix r;
    for(int i = 0; i < 16; i++)
        r.cells[i] = cells[i] + other.cells[i];
    return r;
}
Matrix Matrix::operator-(const Matrix & other) const {
    Matrix r;
    for(int i = 0; i < 16; i++)
        r.cells[i] = cells[i] - other.cells[i];
    return r;
}

Matrix Matrix::operator/(const float  & scalar) const{
    Matrix r;
    for(int i = 0; i < 16; i++)
        r.cells[i] = cells[i] / scalar;
    return r;
}

Vector3 Matrix::Backward() const{
    throw NotImplementedException("Backward");
}
Vector3 Matrix::Down()     const{
    throw NotImplementedException("Down");
}
Vector3 Matrix::Left()     const{
    throw NotImplementedException("Left");
}

Vector3 Matrix::Forward()  const{
    return Vector3(M13(), M23(), M33());
}
Vector3 Matrix::Right()    const{
    return Vector3(M11(), M21(), M31());
}
Vector3 Matrix::Up()       const{
    return Vector3(M12(), M22(), M32());
}
Vector3 Matrix::Translation() const{
    return Vector3(M14(), M24(), M34());
}

void Matrix::setForward(Vector3 vec)  {
    M13(vec.X); M23(vec.Y); M33(vec.Z);
}
void Matrix::setRight(Vector3 vec)    {
    M11(vec.X); M21(vec.Y); M31(vec.Z);
}
void Matrix::setUp(Vector3 vec)       {
    M12(vec.X); M22(vec.Y); M32(vec.Z);
}
void Matrix::setTranslation(Vector3 vec) {
    M14(vec.X); M24(vec.Y); M34(vec.Z);
}
// </editor-fold>
