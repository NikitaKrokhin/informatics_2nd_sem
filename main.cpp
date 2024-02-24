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

void delete_number(figure *number);
void input(figure *number);
void square(figure *number,int i);
void perimeter(figure *number,int i);
void output(figure *number);
void realloc(figure *number,int size_arr);
double get_max_S(figure *number,int size_arr);
double get_max_P(figure *number,int size_arr);
void clean();

int main()
{
    setlocale(LC_CTYPE,"");
    int size_arr,i;
    cout << "������� �������������� ����������?"<<endl;
    cin >> size_arr;

    figure *p_number = new figure[size_arr];



    int num=0;
    do{
        cout<<"1-���� ������ ��������������"<<endl;
        cout<<"2-����� ������ ��������������"<<endl;
        cout<<"3-����� MAX_S � MAX_P"<<endl;
        cout<<"4-�������� ������ � ��������������"<<endl;
        cout<<"0-��������� ������"<<endl;
        cout << "������� ��������: ";
        cin>>num;
        clean();
        if(num==1){
            clean();
            input(p_number);
            clean();
        }
        else if(num==2){
            clean();
            output(p_number);
            cout<<"��� ����������� � ������� ���� ������� ����� �������!"<<endl;
            //�������� ����������� � ���� ��� ������� ����� �������
            cin.get();
            cin.get();
            clean();
        }else if(num==3){
            clean();
            int num_p;
            cout<<"1-����� ������������ ������� ��������������"<<endl;
            cout<<"2-����� ������������� ��������� ��������������"<<endl;
            cout<<"3-�������� � ����"<<endl;
            cin>>num_p;
            clean();
            if(num_p==1){
                cout << "������������ �������:" << get_max_S(p_number,size_arr)<<endl;
                //�������� ����������� � ���� ��� ������� ����� �������
                cout<<"��� ����������� � ������� ���� ������� ����� �������!"<<endl;
                cin.get();
                cin.get();
                clean();
            }else if(num_p==2){
                cout << "������������ ��������:" << get_max_P(p_number,size_arr)<<endl;
                //�������� ����������� � ���� ��� ������� ����� �������
                cout<<"��� ����������� � ������� ���� ������� ����� �������!"<<endl;
                cin.get();
                cin.get();
                clean();
            }
            else{
                clean();
            }
        }else if(num==4){
            delete_number(p_number);
            clean();
        }
    }while(num!=0);
    cout << "������ ���������!"<<endl;

    delete[] p_number;
    return 0;
}
void input(figure *number){
    int i;
    cout << "������ ������ �������������� �������?"<<endl;
    cin >> i;
    i--;
    cout <<"������� ���������� ������\n";
    cin >>number[i].count_side;
    cout <<"������� ����� �������\n";
    cin >>number[i].lenght_side;
    cout <<"������� ���������� x\n";
    cin >>number[i].x;
    cout <<"������� ���������� y\n";
    cin >>number[i].y;

    square(number,i);
    perimeter(number,i);

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
void output(figure *number){
    int i;
    cout << "������ ������ �������������� ��������?"<<endl;
    cin >> i;
    i--;
    cout << "�������������: " << i+1 << "\n";
    cout << "������� S: " << number[i].S << "\n";
    cout << "�������� P: " << number[i].P << "\n";
}
void perimeter(figure *number,int i){
    number[i].P=number[i].lenght_side*number[i].count_side;
}
void realloc(figure *number,int size_arr){
    figure *p_number_new = new figure[size_arr+1];
    cout <<"pn0: " << number[0].S;
    for(int i=0;i<size_arr-1;i++){
        p_number_new[i]=number[i];
    }
    delete[] number;
    number=p_number_new;
    delete[] p_number_new;
    number[1].S=12;
    cout <<"pn1: " << number[1].S;
}

double get_max_S(figure *number,int size_arr){
    int i=0,j=0;
    double *max_number_S=new double[size_arr];
    double max_S=number[i].S;
    for(int i=0;i<size_arr;i++){
        if(number[i].S>max_S){
            max_S=number[i].S;
        }
    }
    for(int i=0;i<size_arr;i++){
        if(number[i].S==max_S){
            max_number_S[j]=i+1;
            j++;
        }
    }
    cout <<"�������������� � ������������ ��������: ";
    for(int k=0;k<j;k++){
        cout << max_number_S[k]<<"\t";
    }
    cout << endl;
    return max_S;
}
double get_max_P(figure *number,int size_arr){
    int i=0,j=0;
    double *max_number_P=new double[size_arr];

    double max_P=number[i].P;
    for(int i=0;i<size_arr;i++){
        if(number[i].P>max_P){
            max_P=number[i].P;
        }
    }
    for(int i=0;i<size_arr;i++){
        if(number[i].P==max_P){
            max_number_P[j]=i+1;
            j++;
        }
    }
    cout <<"�������������� � ������������ ����������: ";
    for(int k=0;k<j;k++){
        cout << max_number_P[k]<<"\t";
    }
    cout << endl;
    return max_P;
}
void clean(){
    int i=0;
    while(i<100){
        cout<<endl;
        i++;
    }
    return;
}
void delete_number(figure *number){
    int i;
    cout << "������ ������ �������������� �������?"<<endl;
            cin>>i;
            i--;
    number[i].count_side=0;
    number[i].lenght_side=0;
    number[i].x=0;
    number[i].y=0;
    number[i].S=0;
    number[i].P=0;
    return;
}
