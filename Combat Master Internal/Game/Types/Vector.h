#pragma once

class Vector4
{
public:
    float x, y, z, w;

    Vector4() : x(), y(), z(), w() {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vector4 operator+(const Vector4& a) const { return Vector4(x + a.x, y + a.y, z + a.z, w + a.w); }
    Vector4 operator-(const Vector4& a) const { return Vector4(x - a.x, y - a.y, z - a.z, w - a.w); }
    Vector4 operator*(const Vector4& a) const { return Vector4(x * a.x, y * a.y, z * a.z, w * a.w); }
    Vector4 operator/(const Vector4& a) const { return Vector4(x / a.x, y / a.y, z / a.z, w / a.w); }

    Vector4 operator+(float s) const { return Vector4(x + s, y + s, z + s, w + s); }
    Vector4 operator-(float s) const { return Vector4(x - s, y - s, z - s, w - s); }
    Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
    Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }

    Vector4& operator+=(const Vector4& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
    Vector4& operator-=(const Vector4& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
    Vector4& operator*=(const Vector4& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this; }
    Vector4& operator/=(const Vector4& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this; }

    Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

    bool operator==(const Vector4& a) const { return x == a.x && y == a.y && z == a.z && w == a.w; }
    bool operator!=(const Vector4& a) const { return !(*this == a); }

    float Length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Vector4 Normalized() const { float len = Length(); return len == 0.0f ? Vector4(0, 0, 0, 0) : *this / len; }
    void Normalize() { float len = Length(); if (len != 0.0f) *this /= len; }

    static float Dot(const Vector4& a, const Vector4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
    static float Distance(const Vector4& a, const Vector4& b) { return (a - b).Length(); }
};

class Vector3
{
public:
    float x, y, z;

    Vector3() : x(), y(), z() {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& a) const { return Vector3(x + a.x, y + a.y, z + a.z); }
    Vector3 operator-(const Vector3& a) const { return Vector3(x - a.x, y - a.y, z - a.z); }
    Vector3 operator*(const Vector3& a) const { return Vector3(x * a.x, y * a.y, z * a.z); }
    Vector3 operator/(const Vector3& a) const { return Vector3(x / a.x, y / a.y, z / a.z); }

    Vector3 operator+(float s) const { return Vector3(x + s, y + s, z + s); }
    Vector3 operator-(float s) const { return Vector3(x - s, y - s, z - s); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

    Vector3& operator+=(const Vector3& a) { x += a.x; y += a.y; z += a.z; return *this; }
    Vector3& operator-=(const Vector3& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
    Vector3& operator*=(const Vector3& a) { x *= a.x; y *= a.y; z *= a.z; return *this; }
    Vector3& operator/=(const Vector3& a) { x /= a.x; y /= a.y; z /= a.z; return *this; }

    Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

    bool operator==(const Vector3& a) const { return x == a.x && y == a.y && z == a.z; }
    bool operator!=(const Vector3& a) const { return !(*this == a); }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 Normalized() const { float len = Length(); return len == 0.0f ? Vector3(0, 0, 0) : *this / len; }
    void Normalize() { float len = Length(); if (len != 0.0f) *this /= len; }

    static float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    static float Distance(const Vector3& a, const Vector3& b) { return (a - b).Length(); }
};

class Vector2
{
public:
    float x, y;

    Vector2() : x(), y() {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& a) const { return Vector2(x + a.x, y + a.y); }
    Vector2 operator-(const Vector2& a) const { return Vector2(x - a.x, y - a.y); }
    Vector2 operator*(const Vector2& a) const { return Vector2(x * a.x, y * a.y); }
    Vector2 operator/(const Vector2& a) const { return Vector2(x / a.x, y / a.y); }

    Vector2 operator+(float s) const { return Vector2(x + s, y + s); }
    Vector2 operator-(float s) const { return Vector2(x - s, y - s); }
    Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
    Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

    Vector2& operator+=(const Vector2& a) { x += a.x; y += a.y; return *this; }
    Vector2& operator-=(const Vector2& a) { x -= a.x; y -= a.y; return *this; }
    Vector2& operator*=(const Vector2& a) { x *= a.x; y *= a.y; return *this; }
    Vector2& operator/=(const Vector2& a) { x /= a.x; y /= a.y; return *this; }

    Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
    Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

    bool operator==(const Vector2& a) const { return x == a.x && y == a.y; }
    bool operator!=(const Vector2& a) const { return !(*this == a); }

    float Length() const { return std::sqrt(x * x + y * y); }
    Vector2 Normalized() const { float len = Length(); return len == 0.0f ? Vector2(0, 0) : *this / len; }
    void Normalize() { float len = Length(); if (len != 0.0f) *this /= len; }

    static float Dot(const Vector2& a, const Vector2& b) { return a.x * b.x + a.y * b.y; }
    static float Distance(const Vector2& a, const Vector2& b) { return (a - b).Length(); }

    
    FORCEINLINE ImVec2 ToImVec()
    {
        return ImVec2(x, y);
    }
};