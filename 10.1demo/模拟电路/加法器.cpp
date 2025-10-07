# include <iostream>


using namespace std;

// 基本原件


// 晶体管
bool npn(bool c, bool b){
    bool e = false;
    if (c==true && b == true){
        return true;
    }
    return e;
}

bool And(bool c, bool i1, bool i2){
    // 两个npn串联
    bool s1 = npn(c,i1);
    bool s2 = npn(s1,i2);
    return s2;
}   

bool Or(bool c, bool i1, bool i2){
    // 两个npn并联
    bool s1 = npn(c, i1);
    bool s2 = npn(c, i2);
    
    if (s1 == true or s2 == true) return true;
    return false;
}

bool Not(bool c){
    return !c;
}


// 初级原件
bool Xor(bool c, bool i1, bool i2){
    bool s1 = And(c, !i2, i1);
    bool s2 = And(c ,i2, !i2);
    bool out = Or(c ,s1, s2);
    return out;
}

bool HalfAdder(bool c, bool i1, bool i2){
    bool o1 = Xor(c , i1, i2);
    cout << o1 << "  ";
    bool carray = And(c, i1, i2);
    cout << carray << endl;
    return 0;
}

struct FAres
{
    bool sum;
    bool cy;
};

FAres FullAdder(bool c, bool i1, bool i2, bool cy){
    // bool s1 = Xor(c, i1 ,i2);
    // bool o1 = Xor(c, s1, cy);
    bool o1 = i1 ^ i2 ^ cy;
    // 记录第一次进位
    bool c1 = And(c ,i1, i2);
    
    // 记录第二次进位

    bool c2 = And(c, i1^i2, c1);

    bool o2 = Or(c, c1,c2);

    return {o1, o2};
}

struct caculator_8b(){

}

int main(){
    bool c = true;
    FAres r = FullAdder(c, 1,1,1);
    cout << r.sum << "   " << r.cy << endl;
}