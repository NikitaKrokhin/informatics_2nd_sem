#include <iostream>
#include <math.h>

using namespace std;


struct figure{
    int count_side;
    double lenght_side;
    double S;
    double P;
    double x;
    double y;
};

void input(figure *number,int i);
void square(figure *number,int i);
void perimeter(figure *number,int i);
void output(figure *number,int i);
void realloc(figure *p_number,int size1);

int main()
{
    setlocale(LC_CTYPE,"");
    int size1;
    cin >> size1;

    figure *p_number = new figure[size1];

//    realloc(p_number,size1);
//    cout << "121 " << p_number[1].count_side << "\n";
    input(p_number,0);
//    cout << "121 " << p_number[1].count_side << "\n";
    square(p_number,0);
    perimeter(p_number,0);
    output(p_number,0);
    realloc(p_number,size1);
    cout << "pn0: " << p_number[0].S;

    return 0;
}
void input(figure *number,int i){
    cout <<"������� ���������� ������\n";
    cin >>number[i].count_side;
    cout <<"������� ����� �������\n";
    cin >>number[i].lenght_side;
    cout <<"������� ���������� x\n";
    cin >>number[i].x;
    cout <<"������� ���������� y\n";
    cin >>number[i].y;
    return;
}
void square(figure *number,int i){
    if(number[i].count_side==3){
        number[i].S=(number[i].lenght_side*number[i].lenght_side)*sqrt(3)/2;
    }
    else if(number[i].count_side==4){
        number[i].S=(number[i].lenght_side*number[i].lenght_side);
    }
    else{
       number[i].S=(number[i].count_side*(number[i].lenght_side*number[i].lenght_side))/(4*tan(M_PI/number[i].count_side));
    }
}
void output(figure *number,int i){
    cout << "�������������: " << i+1 << "\n";
    cout << "������� S: " << number[i].S << "\n";
    cout << "�������� P: " << number[i].P << "\n";
}
void perimeter(figure *number,int i){
    number[i].P=number[i].lenght_side*number[i].count_side;
}
void realloc(figure *p_number,int size1){
    figure *p_number_new = new figure[size1+1];
    cout <<"pn0: " << p_number[0].S;
    for(int i=0;i<size1-1;i++){
        p_number_new[i]=p_number[i];
    }
    delete[] p_number;
    p_number=p_number_new;
    delete[] p_number_new;
    p_number[1].S=12;
    cout <<"pn1: " << p_number[1].S;
}
