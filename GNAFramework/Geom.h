#ifndef GEOM_H
#define	GEOM_H

#include <math.h>
#include <stdio.h>

namespace GNAFramework {
    
    struct Vector2;

    typedef struct RectangleF {
    public:
        float X;
        float Y;
        float width;
        float height;

        /** Retorna un rectangulo con todas sus componentes en 0 */
        const static RectangleF Empty;

        static RectangleF Intersect(RectangleF value1, RectangleF value2);
        static void       Intersect(const RectangleF *value1, const RectangleF *value2, RectangleF *result);
        static RectangleF Union(RectangleF value1, RectangleF value2);
        static void       Union(const RectangleF *value1, const RectangleF *value2, RectangleF *result);


        RectangleF();

        RectangleF(float x, float y, float width, float height);

        void setValue(float x, float y, float width, float height);


        bool operator==(const RectangleF & other) const;
        bool operator!=(const RectangleF & other) const;

        inline float Bottom() const { return Y + height; }
        inline float Left()   const { return X;          }
        inline float Right()  const { return X + width;  }
        inline float Top()    const { return Y;          }

        Vector2 Center() const;

        bool IsEmpty() const;

        Vector2 getLocation() const;
        void setLocation(Vector2 value);

        bool Contains(Vector2 value) const;
        bool Contains(RectangleF value) const;
        bool Contains(float x, float y) const;
        void Contains(const Vector2 *value, bool *result) const;
        void Contains(const RectangleF *value, bool *result) const;

        void Inflate(int horizontalAmount, int verticalAmount);

        
        
        bool Intersects(RectangleF value) const;
        void Intersects(const RectangleF *value, bool *result) const;
        void Offset(Vector2 amount);
        void Offset(int offsetX, int offsetY);




        //RectangleF(const RectangleF& orig);
        virtual ~RectangleF();
    private:

    } RectangleF;
    
    
    typedef struct Circle {
    public:
        float X;
        float Y;
        float radio;

        /** Retorna un circulo con todas sus componentes en 0 */
        const static Circle Empty;
        /** Retorna un circulo en el origen de radio 1 */
        const static Circle Unitary;
        
        
        //static RectangleF Union(RectangleF value1, RectangleF value2);
        //static void Union(const RectangleF *value1, const RectangleF *value2, RectangleF *result);


        Circle();

        Circle(float x, float y, float radio);

        void setValue(float x, float y, float radio);


        bool operator==(const Circle & other) const;
        bool operator!=(const Circle & other) const;

        inline int Bottom() const { return Y + radio; }

        inline int Left()   const { return X - radio; }

        inline int Right()  const { return X + radio; }

        inline int Top()    const { return Y - radio; }

        Vector2 Center() const;

        bool IsEmpty() const;

        Vector2 getLocation() const;
        void setLocation(Vector2 value);
        
        void toRectangle(RectangleF *rec) const;
        RectangleF toRectangle() const;

        bool Contains(Vector2 value) const;
        //bool Contains(Circle value) const;
        bool Contains(float x, float y) const;
        void Contains(const Vector2 *value, bool *result) const;
        //void Contains(const Circle *value, bool *result) const;
        
        void Inflate(int amount);

        
        
        bool Intersects(Circle value) const;
        void Intersects(const Circle *value, bool *result) const;
        bool Intersects(RectangleF value) const;
        void Intersects(const RectangleF *value, bool *result) const;
        void Offset(Vector2 amount);
        void Offset(int offsetX, int offsetY);

        //RectangleF(const RectangleF& orig);
        virtual ~Circle();
    private:
        float distance(float x, float y) const;
        float distance(Vector2 vec) const;
    } Circle;


    typedef struct {
        int X;
        int Y;
        int Z;
    } Point;

    struct Vector2 {
    public:
        float X;
        float Y;

        const static Vector2 Zero;
        const static Vector2 UnitX;
        const static Vector2 UnitY;
        const static Vector2 One;

        inline static void  Dot(const Vector2* value1, const Vector2* value2, float *result) 
        { *result = (value1->X * value2->X) + (value1->Y * value2->Y); }
        static float Dot(Vector2 value1, Vector2 value2);
        
        inline static void Normalize(const Vector2* value, Vector2 *result)
        { float l = value->Length(); *result = Vector2(value->X / l, value->Y / l); }
        
        inline static Vector2 Normalize(Vector2 value)
        { float l = value.Length(); return Vector2(value.X / l, value.Y / l); }
        
        Vector2();
        Vector2(float X, float Y);
        
        void setVal(float X, float Y);

        void rotate(Vector2 Center, float ang);
        
        inline float Length() const{ return sqrt(X*X + Y*Y); }
        
        inline float LengthSquared() const{ return X*X + Y*Y; }
        
        Vector2 operator*(const Vector2 & other) const
        { return Vector2(X * other.X, Y * other.Y); }
        Vector2 operator*(const float  & scalar) const 
        { return Vector2(X * scalar, Y * scalar); }
        
        Vector2 operator/(const Vector2 & other) const
        { return Vector2(X/other.X, Y/other.Y); }
        Vector2 operator/(const float  & scalar) const 
        { return Vector2(X/scalar, Y/scalar); }
        
        Vector2 operator+(const Vector2 & other) const
        { return Vector2(X + other.X, Y + other.Y); }
        Vector2 operator-(const Vector2 & other) const
        { return Vector2(X - other.X, Y - other.Y); }
        
        Vector2 *operator+=(const Vector2 & other)
        { X += other.X; Y += other.Y;  return this; }
        Vector2 *operator-=(const Vector2 & other)
        { X -= other.X; Y -= other.Y;  return this; }
        
        Vector2 *operator*=(const Vector2 & other)
        { X *= other.X; Y *= other.Y;  return this; }
        Vector2 *operator*=(const float  & scalar)
        { X *= scalar; Y *= scalar;  return this; }
        
        Vector2 *operator/=(const Vector2 & other)
        { X /= other.X; Y /= other.Y;  return this; }
        Vector2 *operator/=(const float  & scalar)
        { X /= scalar; Y /= scalar; return this; }
        
        bool     operator==(const Vector2 & other) const
        { return (X == other.X) && (Y == other.Y); }
    };

    struct Vector3 {
    public:
        float X;
        float Y;
        float Z;
        const static Vector3 Zero;
        const static Vector3 One;
        
        const static Vector3 UnitX;
        const static Vector3 UnitY;
        const static Vector3 UnitZ;
        
        const static Vector3 Backward;
        const static Vector3 Down;
        const static Vector3 Forward;
        const static Vector3 Left;
        const static Vector3 Right;
        const static Vector3 Up;
        
        inline static void  Dot(const Vector3* value1, const Vector3* value2, float *result) 
        { *result = (value1->X * value2->X) + (value1->Y * value2->Y) + (value1->Z * value2->Z); }
        static float Dot(Vector3 value1, Vector3 value2)
        { float r; Dot(&value1, &value2, &r); return r; }
        
        static void  Cross(const Vector3* value1, const Vector3* value2, Vector3 *result);
        static Vector3 Cross(Vector3 value1, Vector3 value2);
        
        inline static void Normalize(const Vector3* value, Vector3 *result)
        { float l = value->Length(); *result = Vector3(value->X / l, value->Y / l, value->Z / l); }
        
        inline static Vector3 Normalize(Vector3 value)
        { float l = value.Length(); return Vector3(value.X / l, value.Y / l, value.Z / l); }
        
        inline static Vector3 Reflect(Vector3 normal, Vector3 incident) {
            return incident - normal * (2.f * Dot(normal, incident));
        }
        
        Vector3();
        
        Vector3(float X, float Y, float Z);

        void setVal(float X, float Y, float Z);
        
        inline float Length() const{ return sqrt(X*X + Y*Y + Z*Z); }
        
        inline float LengthSquared() const{ return X*X + Y*Y + Z*Z; }
        
        Vector3 operator*(const Vector3 & other) const
        { return Vector3(X*other.X, Y*other.Y, Z*other.Z); }
        Vector3 operator*(const float  & scalar) const 
        { return Vector3(X*scalar, Y*scalar, Z*scalar); }
        
        Vector3 operator/(const Vector3 & other) const
        { return Vector3(X/other.X, Y/other.Y, Z/other.Z); }
        Vector3 operator/(const float  & scalar) const 
        { return Vector3(X/scalar, Y/scalar, Z/scalar); }
        
        Vector3 operator+(const Vector3 & other) const
        { return Vector3(X+other.X, Y+other.Y, Z+other.Z); }
        Vector3 operator-(const Vector3 & other) const
        { return Vector3(X-other.X, Y-other.Y, Z-other.Z); }
        
        Vector3 *operator+=(const Vector3 & other)
        { X += other.X; Y += other.Y; Z += other.Z; return this; }
        Vector3 *operator-=(const Vector3 & other)
        { X -= other.X; Y -= other.Y; Z -= other.Z; return this; }
        
        Vector3 *operator*=(const Vector3 & other)
        { X *= other.X; Y *= other.Y; Z *= other.Z; return this; }
        Vector3 *operator*=(const float  & scalar)
        { X *= scalar; Y *= scalar; Z *= scalar; return this; }
        
        Vector3 *operator/=(const Vector3 & other)
        { X /= other.X; Y /= other.Y; Z /= other.Z; return this; }
        Vector3 *operator/=(const float  & scalar)
        { X /= scalar; Y /= scalar; Z /= scalar; return this; }
        
        bool     operator==(const Vector3 & other) const
        { return (X == other.X) && (Y == other.Y) && (Z == other.Z); }
        
        void print() const { printf("<%f, %f, %f>", X, Y, Z); }
    
    };

    struct Vector4 {
    public:
        float X;
        float Y;
        float Z;
        float W;

        Vector4();
        Vector4(float X, float Y, float Z, float W);

        void setVal(float X, float Y, float Z, float W);
        
        Vector4 operator*(const Vector4 & other) const
        { return Vector4(X*other.X, Y*other.Y, Z*other.Z, W*other.W); }
        Vector4 operator*(const float  & scalar) const 
        { return Vector4(X*scalar, Y*scalar, Z*scalar, W*scalar); }
        
        Vector4 operator/(const Vector4 & other) const
        { return Vector4(X/other.X, Y/other.Y, Z/other.Z, W/other.W); }
        Vector4 operator/(const float  & scalar) const 
        { return Vector4(X/scalar, Y/scalar, Z/scalar, W/scalar); }
        
        Vector4 operator+(const Vector4 & other) const
        { return Vector4(X+other.X, Y+other.Y, Z+other.Z, W+other.W); }
        Vector4 operator-(const Vector4 & other) const
        { return Vector4(X-other.X, Y-other.Y, Z-other.Z, W-other.W); }
        
        Vector4 *operator+=(const Vector4 & other)
        { X += other.X; Y += other.Y; Z += other.Z; W += other.W; return this; }
        Vector4 *operator-=(const Vector4 & other)
        { X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W; return this; }
        
        
        Vector4 *operator*=(const Vector4 & other)
        { X *= other.X; Y *= other.Y; Z *= other.Z; W *= other.W; return this; }
        Vector4 *operator*=(const float  & scalar)
        { X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; return this; }
        
        Vector4 *operator/=(const Vector4 & other)
        { X /= other.X; Y /= other.Y; Z /= other.Z; W += other.W; return this; }
        Vector4 *operator/=(const float  & scalar)
        { X /= scalar; Y /= scalar; Z /= scalar; W += scalar; return this; }
    };
    
    struct Frustum {
    public:
        Frustum(){
            fovY = 45.f;
            aspectRatio = 4.f/3.f;
            front = 0.1f;
            back  = 100.f;
        }

        Frustum(float fovY, float aspectRatio, float front, float back){
            this->fovY = fovY;
            this->aspectRatio = aspectRatio;
            this->front = front;
            this->back  = back;
        }

        float getFovY()       const { return fovY;        }
        float getAspectRatio()const { return aspectRatio; }
        float getFront()      const { return front;       }
        float getBack()       const { return back;        }

    private:
        float fovY;
        float aspectRatio;
        float front;
        float back;

    };

    
    struct Matrix {
    public:
        const static Matrix Identity;
        const static Matrix Ones;
        const static Matrix Zeros;
        
        static Matrix Lerp(Matrix *matrix1, Matrix *matrix2, float amount);
        static Matrix CreateLookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector);
        
        static Matrix CreatePerspective(
                float width,
                float height,
                float nearPlaneDistance,
                float farPlaneDistance
        );
        
        static Matrix CreatePerspective(Frustum frustum);
        
        static Matrix CreatePerspectiveFieldOfView(
                float fieldOfView,
                float aspectRatio,
                float nearPlaneDistance,
                float farPlaneDistance
        );
        
        static Matrix CreateWorld(
                Vector3 position,
                Vector3 forward,
                Vector3 up
        );
        
        static Matrix CreateScale(float scale);
        static void   CreateScale(float scale, Matrix *result);
        static Matrix CreateScale(Vector3 scale);
        static void   CreateScale(Vector3 scale, Matrix *result);
        static Matrix CreateScale(float scaleX, float scaleY, float scaleZ);
        static void   CreateScale(float scaleX, float scaleY, float scaleZ, Matrix *result);
        
        
        static Matrix CreateTranslation(Vector3 translation);
        static void   CreateTranslation(Vector3 translation, Matrix *result);
        static Matrix CreateTranslation(float translationX, float translationY, float translationZ);
        static void   CreateTranslation(float translationX, float translationY, float translationZ, Matrix *result);
        
        static void   CreateRotationX(float radians, Matrix *result);
        static Matrix CreateRotationX(float radians);
        
        static void   CreateRotationY(float radians, Matrix *result);
        static Matrix CreateRotationY(float radians);
        
        static void   CreateRotationZ(float radians, Matrix *result);
        static Matrix CreateRotationZ(float radians);
        
        
        static Matrix Invert();
        
        inline float M11() const{ return M(0, 0); }
        inline float M12() const{ return M(0, 1); }
        inline float M13() const{ return M(0, 2); }
        inline float M14() const{ return M(0, 3); }

        inline float M21() const{ return M(1, 0); }
        inline float M22() const{ return M(1, 1); }
        inline float M23() const{ return M(1, 2); }
        inline float M24() const{ return M(1, 3); }

        inline float M31() const{ return M(2, 0); }
        inline float M32() const{ return M(2, 1); }
        inline float M33() const{ return M(2, 2); }
        inline float M34() const{ return M(2, 3); } 

        inline float M41() const{ return M(3, 0); }
        inline float M42() const{ return M(3, 1); }
        inline float M43() const{ return M(3, 2); }
        inline float M44() const{ return M(3, 3); }
        
        
        
        inline void M11(float v) { M(0, 0, v); }
        inline void M12(float v) { M(0, 1, v); }
        inline void M13(float v) { M(0, 2, v); }
        inline void M14(float v) { M(0, 3, v); }

        inline void M21(float v) { M(1, 0, v); }
        inline void M22(float v) { M(1, 1, v); }
        inline void M23(float v) { M(1, 2, v); }
        inline void M24(float v) { M(1, 3, v); }

        inline void M31(float v) { M(2, 0, v); }
        inline void M32(float v) { M(2, 1, v); }
        inline void M33(float v) { M(2, 2, v); }
        inline void M34(float v) { M(2, 3, v); } 

        inline void M41(float v) { M(3, 0, v); }
        inline void M42(float v) { M(3, 1, v); }
        inline void M43(float v) { M(3, 2, v); }
        inline void M44(float v) { M(3, 3, v); }

        Matrix();
        Matrix(const float values[16]);
        
        /**
         * 
         * @param i on [0, 3]
         * @param j on [0, 3]
         * @return 
         */
        inline float M(int i, int j) const{ return cells[4*j + i]; }
        
        void print() const {
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 4; j++){
                    printf("%f, ", this->M(i,j));
                }
                printf("\n");
            }
        }
        
        /**
         * Return a pointer to the specified position of the matrix.
         * @param i on [0, 3]
         * @param j on [0, 3]
         */
        inline float *M_ptr(int i, int j) { return cells + (4*j + i); }
        
        inline void M(int i, int j, float value){ cells[4*j + i] = value; }
        
        void values(double *arr) {
            for(int i = 0; i < 16; i++) {
                arr[i] = static_cast<double>(cells[i]);
            }
        }
        
        
        bool operator==(const Matrix & other) const;
        bool operator!=(const Matrix & other) const;
        
        Matrix  operator*(const Matrix  & other ) const;
        Vector4 operator*(const Vector4 & vec   ) const;
        Vector3 operator*(const Vector3 & vec   ) const;
        Matrix  operator*(const float   & scalar) const;
        
        Matrix operator+(const Matrix & other) const;
        Matrix operator-(const Matrix & other) const;
        
        Matrix operator/(const float  & scalar) const;
        
        Vector3 Backward() const;
        Vector3 Down()     const;
        Vector3 Forward()  const;
        Vector3 Left()     const;
        Vector3 Right()    const;
        Vector3 Up()       const;
        Vector3 Translation() const;
        
        
        void setForward(Vector3 vec);
        void setRight(Vector3 vec);
        void setUp(Vector3 vec);
        void setTranslation(Vector3 vec);
        
        
    private:
        float cells[16];
        static const float iValues[16];
        static const float onesValues[16];
        static const float zerosValues[16];
    };
    
    
    struct BoundingSphere {
    public:
        float X, Y, Z;
        float R;

        BoundingSphere(){
            X = 0.f; Y = 0.f; Z = 0.f; R = 1.f;
        }

        BoundingSphere(Vector3 pos, float radio){
            X = pos.X; Y = pos.Y; Z = pos.Z; R = radio;
        }

        BoundingSphere(float x, float y, float z, float radio){
            X = x; Y = y; Z = z; R = radio;
        }

        Vector3 Center() const { return Vector3(X,Y,Z); }

        void setCenter(Vector3 center) {
            X = center.X; Y = center.Y; Z = center.Z;
        }

        float Radio()const { return R; }

        bool Intersects(BoundingSphere value) const{
            float dX = X - value.X, dY = Y - value.Y, dZ = Z - value.Z;
            float dR = R + value.R;
            return dX*dX + dY*dY + dZ*dZ <= dR * dR;
        }

        bool Contains(Vector3 value) const{
            float dX = X - value.X, dY = Y - value.Y, dZ = Z - value.Z;
            return dX*dX + dY*dY + dZ*dZ <= R*R;
        }

        virtual ~BoundingSphere() {}
    private:

    };
}

#endif	/* GEOM_H */

