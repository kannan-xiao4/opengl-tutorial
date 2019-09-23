#pragma once
#include <algorithm>
#include <cmath>
#include <GL/glew.h>

class Matrix{
    GLfloat matrix[16];

public:
    Matrix(){}
    
    Matrix(const GLfloat *a){
        std::copy(a, a + 16, matrix);
    }
    
    const GLfloat *data() const{
        return matrix;
    }
    
    // 随伴行列を転置した行列を求めている つまり p.203 の(79) 式 G　を求める
    void getNormalMatrix(GLfloat *m) const{
        m[0] = matrix[5] * matrix[10] - matrix[6] * matrix[9];
        m[1] = matrix[6] * matrix[8] - matrix[4] * matrix[10];
        m[2] = matrix[4] * matrix[9] - matrix[5] * matrix[8];
        m[3] = matrix[9] * matrix[2] - matrix[10] * matrix[1]; // -dyx = - (axy * azz - azy * axz) = azy * axz - axy * azz
        m[4] = matrix[10] * matrix[0] - matrix[8] * matrix[2];
        m[5] = matrix[8] * matrix[1] - matrix[9] * matrix[0];
        m[6] = matrix[1] * matrix[6] - matrix[2] * matrix[5];
        m[7] = matrix[2] * matrix[4] - matrix[0] * matrix[6];
        m[8] = matrix[0] * matrix[5] - matrix[1] * matrix[4];
    }
    
    void loadIdentity(){
        std::fill(matrix, matrix + 16, 0.0f); //行列の要素をすべて0にする
        matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f; // 対角成分にだけ 1 を入れる
    }

    // 単位行列を作成する
    static Matrix identity(){
        Matrix t;
        t.loadIdentity();
        return t;
    }
    
    // x,y,z だけ平行移動する変換行列を作成する
    static Matrix translate(GLfloat x, GLfloat y, GLfloat z){
        Matrix t;
        t.loadIdentity();
        t.matrix[12] = x;
        t.matrix[13] = y;
        t.matrix[14] = z;
        
        return t;
    }
    
    // x, y, z 倍に拡大縮小する変換行列を作成する。
    static Matrix scale(GLfloat x, GLfloat y, GLfloat z){
        Matrix t;
        t.loadIdentity();
        t.matrix[0] = x;
        t.matrix[5] = y;
        t.matrix[10] = z;
        
        return t;
    }

    // y軸の値に係数をかけたものをx軸に足した場合によるせん断の変換行列を作成する
    static Matrix shear_xy(GLfloat s){
        Matrix t;
        t.loadIdentity();
        t.matrix[4] = s;
        
        return t;
    }
    
    // 座標軸中心の回転変換行列

    static Matrix rotate_x(GLfloat theta){
        Matrix t;
        t.loadIdentity();
        t.matrix[5] = cos(theta);
        t.matrix[6] = sin(theta);
        t.matrix[9] = -sin(theta);
        t.matrix[10] = cos(theta);

        return t;
    }
    
    static Matrix rotate_y(GLfloat theta){
        Matrix t;
        t.loadIdentity();
        t.matrix[0] = cos(theta);
        t.matrix[2] = -sin(theta);
        t.matrix[8] = -sin(theta);
        t.matrix[10] = cos(theta);
        
        return t;
    }
    
    static Matrix rotate_z(GLfloat theta){
        Matrix t;
        t.loadIdentity();
        t.matrix[0] = cos(theta);
        t.matrix[1] = sin(theta);
        t.matrix[4] = -sin(theta);
        t.matrix[5] = cos(theta);
        
        return t;
    }

    static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z){
        Matrix t;
        const GLfloat d(sqrt(x*x+y*y+z*z));

        if (d > 0.0f) {
            const GLfloat l(x/d), m(y/d), n(z/d);
            const GLfloat l2(l*l), m2(m*m), n2(n*n);
            const GLfloat lm(l*m), mn(m*n), nl(n*l);
            const GLfloat c(cos(a)), c1(1.0f-c), s(sin(a));
            
            t.loadIdentity();
            t.matrix[0] = (1.0f - l2) * c + l2;
            t.matrix[1] = lm * c1 + m * s;
            t.matrix[2] = nl * c1 - m * s;
            t.matrix[4] = lm * c1 - n * s;
            t.matrix[5] = (1.0f - m2) * c + m2;
            t.matrix[6] = mn * c1 + l * s;
            t.matrix[8] = nl * c1 + m * s;
            t.matrix[9] = mn * c1 - l * s;
            t.matrix[10] = (1.0f - n2) * c + n2;
        }
        
        return t;
    }
    
    static Matrix lookat(
        GLfloat ex, GLfloat ey, GLfloat ez, //視点の位置
        GLfloat gx, GLfloat gy, GLfloat gz, //目標点の位置
        GLfloat ux, GLfloat uy, GLfloat uz // 上方向のベクトル
     ){
        const Matrix tv(translate(-ex, -ey, -ez));
        
        // t 軸 = e - g
        const GLfloat tx(ex - gx);
        const GLfloat ty(ey - gy);
        const GLfloat tz(ez - gz);
        // r 軸 = u x t 軸
        const GLfloat rx(uy * tz - uz * ty);
        const GLfloat ry(uz * tx - ux * tz);
        const GLfloat rz(ux * ty - uy * tx);
        // s 軸 = t 軸 x r 軸
        const GLfloat sx(ty * rz - tz * ry);
        const GLfloat sy(tz * rx - tx * rz);
        const GLfloat sz(tx * ry - ty * rx);

        const GLfloat s2(sx*sx+sy*sy+sz*sz);
        
        if (s2 == 0.0f) {
            return tv;
        }
        
        Matrix rv;
        rv.loadIdentity();
        
        const GLfloat r(sqrt(rx*rx+ry*ry+rz*rz));
        rv.matrix[0] = rx/r;
        rv.matrix[4] = ry/r;
        rv.matrix[8] = rz/r;
        
        const GLfloat s(sqrt(s2));
        rv.matrix[1] = sx/s;
        rv.matrix[5] = sy/s;
        rv.matrix[9] = sz/s;
        
        const GLfloat t(sqrt(tx*tx+ty*ty+tz*tz));
        rv.matrix[2] = tx/t;
        rv.matrix[6] = ty/t;
        rv.matrix[10] = tz/t;

        return rv * tv;
    }
    
    static Matrix orthogonal(
        GLfloat left, GLfloat right,
        GLfloat bottom, GLfloat top,
        GLfloat zNear, GLfloat zFar
    ){
        Matrix t;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);
        
        if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
            t.loadIdentity();
            t.matrix[0] = 2.0f / dx;
            t.matrix[5] = 2.0f / dy;
            t.matrix[10] = -2.0f / dz;
            t.matrix[12] = -(right + left) / dx;
            t.matrix[13] = -(top + bottom) / dy;
            t.matrix[14] = -(zFar + zNear) / dz;
        }
        
        return t;
    }
    
    static Matrix frustum(
      GLfloat left, GLfloat right,
      GLfloat bottom, GLfloat top,
      GLfloat zNear, GLfloat zFar
    ){
        Matrix t;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);
        
        if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
            t.loadIdentity();
            t.matrix[0] = 2.0f * zNear / dx;
            t.matrix[5] = 2.0f * zNear / dy;
            t.matrix[8] = (right + left) / dx;
            t.matrix[9] = (top + bottom) / dy;
            t.matrix[10] = -(zFar + zNear) / dz;
            t.matrix[11] = -1.0f;
            t.matrix[14] = -2.0f * zFar * zNear / dz;
            t.matrix[15] = 0.0f;
        }
        
        return t;
    }
    
    static Matrix perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar){
        Matrix t;
        const GLfloat dz(zFar - zNear);
        
        if (dz != 0.0f) {
            t.loadIdentity();
            t.matrix[5] = 1.0f / tan(fovy * 0.5f);
            t.matrix[0] = t.matrix[5] /aspect;
            t.matrix[10] = -(zFar + zNear) / dz;
            t.matrix[11] = -1.0f;
            t.matrix[14] = -2.0f * zFar * zNear /dz;
            t.matrix[15] = 0.0f;
        }
        
        return t;
    }
    
    Matrix operator*(const Matrix&m) const{
        Matrix t;
        
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                const int ji(j*4+i);
                
                t.matrix[ji] = 0.0f;
                for (int k = 0; k < 4; k++) {
                    t.matrix[ji] += matrix[k*4+i] * m.matrix[j*4+k];
                }
            }
        }
        
        return t;
    }
};
