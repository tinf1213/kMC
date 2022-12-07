#include<bits/stdc++.h>
using namespace std;
double concentrate_ZnO, concentrate_O3_aq = 55.2 / pow(10, 6), concentrate_H2O2 = 0, concentrate_O2 = 0, concentrate_Odot = 0, concentrate_O3_ad = 0, concentrate_H2O = 55.5;
double delta_time;
long long MC_counter = 0;
const double ktot = 9300 + 740 + 3150 + 14400 + 0.075 + 420000000;
double time_accumulated = 0;
vector<double> react_const = {9300, 740, 3150, 14400, 0.075, 420000000};
vector<double> Cumulated_const(6,0);

/*
define react
r0 => O3+ZnO->O3
r1 => O3->ZnO+O3
r2 => O3->Odot+O2
r3 => O2+Odot->O3
r4 => Odot+H2O->H2O2
r5 => H2O2->2HOdot+ZnO
*/

void calculate_Rates(vector<double>& Rates){
    Rates[0] = react_const[0] * concentrate_O3_aq * concentrate_ZnO;
    Rates[1] = react_const[1] * concentrate_O3_ad;
    Rates[2] = react_const[2] * concentrate_O3_ad;
    Rates[3] = react_const[3] * concentrate_Odot * concentrate_O2;
    Rates[4] = react_const[4] * concentrate_Odot * concentrate_H2O;
    Rates[5] = react_const[5] * concentrate_H2O2 * concentrate_ZnO;
    //for(int i=0;i<6;i++) cout << Rates[i] << endl;
}
void calculate_Cumulated_const(){
    Cumulated_const[0] = react_const[0];
    for(int i=1;i<6;i++){
        Cumulated_const[i] = Cumulated_const[i-1] + react_const[i];
    }
}

int select_event(vector<double>& Cumulated_const){
    double u1 = (double)rand()/RAND_MAX;
    double target = u1 * ktot;
    if(Cumulated_const[0] > target) return 0;
    for(int i=1;i<6;i++){
        if(Cumulated_const[i-1]<target && Cumulated_const[i] >= target) return i;
    }
}
void update_time(){
    double u2 = (double)rand()/RAND_MAX;
    //cout << ktot << endl;
    delta_time = -1*log(u2)/ktot;
    //cout << delta_time << endl;
}
void update_concentrate(int &event, vector<double> &Rates){
    double delta_concentrate = Rates[event] * (delta_time/60);
    if(event == 0){
        concentrate_O3_aq -= delta_concentrate;
        concentrate_O3_ad += delta_concentrate;
    }
    else if(event == 1){
        concentrate_O3_aq += delta_concentrate;
        concentrate_O3_ad -= delta_concentrate;
    }
    else if(event == 2){
        concentrate_O3_ad -= delta_concentrate;
        concentrate_Odot += delta_concentrate;
        concentrate_O2 += delta_concentrate;
    }
    else if(event == 3){
        concentrate_O3_ad += delta_concentrate;
        concentrate_Odot -= delta_concentrate;
        concentrate_O2 -= delta_concentrate;
    }
    else if(event == 4){
        concentrate_Odot -= delta_concentrate;
        concentrate_H2O -= delta_concentrate;
        concentrate_H2O2 += delta_concentrate;
    }
    else if(event == 5){
        concentrate_H2O2 -= delta_concentrate;
    }
}
void init_dat(double& ZnO_weigth){
    cout << "#MC-step   times(s)       [O3](um)" << endl;
    cout << 0 << "          " << "0.000" << "          " << 55.2 << endl;
}
int main(){
    srand(time(NULL));                                                             
    double ZnO_weigth = 10; //cin >> ZnO_weigth;  
    init_dat(ZnO_weigth);               
    concentrate_ZnO = ZnO_weigth / 81.38 * pow(10, 3);
    int MC_counter_accumulated = 0;
    calculate_Cumulated_const();
    while(1){
        vector<double> Rates(6, 0);
        vector<double> Cumulated_Rates(6, 0);
        calculate_Rates(Rates);
        int event = select_event(Cumulated_const);
        //cout << event << endl;
        update_time();
        time_accumulated += delta_time;
        update_concentrate(event, Rates);
        //cout << time_accumulated << endl;
        //cout << delta_time << endl;
        MC_counter++;
        if(MC_counter == 100){
            MC_counter_accumulated += MC_counter;
            cout << MC_counter_accumulated << "        " << time_accumulated << "    " << (concentrate_O3_aq) * pow(10, 6) << endl;
            MC_counter = 0;
        }
        //if(time_accumulated >= 1800) break;
        //if(MC_counter >= 10) break;
        if(MC_counter_accumulated >= 2000) break;
        // break;
    }
    return 0;
}