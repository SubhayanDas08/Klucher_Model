#include <iostream>
#include <fstream>
#include<cmath>

using namespace std;

struct time_date {
   int tm_sec;   // seconds of minutes from 0 to 61
   float tm_min;   // minutes of hour from 0 to 59
   float tm_hour;   // hours of day from 0 to 24
   int tm_mday;  // day of month from 1 to 31
   int tm_mon;   // month of year from 0 to 11
   int tm_year;  // year since 1900
   int tm_wday;  // days since sunday
   int tm_yday;  // days since January 1st
   int tm_isdst; // hours of daylight savings time
};

struct constants{
    float A;
    float B;
    float C;
};

constants val[12]={
                    {1202, 0.141, 0.103}, {1187, 0.142, 0.104}, {1164, 0.149, 0.109}, {1130, 0.164, 0.120}, {1106, 0.177, 0.130}, {1092, 0.185, 0.137},
                    {1093, 0.186, 0.138}, {1107, 0.182, 0.134}, {1136, 0.165, 0.121}, {1136, 0.152, 0.111}, {1190, 0.144, 0.106}, {1204, 0.141, 0.103}
                    };
bool checkYear(int year) 
        { 
            if (year % 400 == 0) 
                return true; 
        
            if (year % 100 == 0) 
                return false; 
        
            if (year % 4 == 0) 
                return true; 
            return false; 
        }

void ashrae(time_date date, time_date time, float lat)
{
    float n = 0;        //day of the year
    float hra = 0;      //hour angle
    float dec = 0;      //declination angle
    float tilt = 0;     //tilt angle
    float azi = 0;      //azimuth angle
    float cosQ = 0;     //angle of incidence of solar radiation
    float cosQz = 0;    //zenith angle
    int interval = 0;   //interval of mins for data representation

    float Ig = 0;       //Hourly Global Radiation
    float Ib = 0;       //Hourly Beam Radiation
    float Id = 0;       //Hourly Diffused Radiation

    cout<<"\nEnter the tilt angle : ";
    cin>>tilt;
    cout<<"\nEnter the azimuth angle : ";
    cin>>azi;

    //Converting degrees into radian
    lat *=3.14159/180;
    tilt *=3.14159/180;
    azi *=3.14159/180;

    cout<<"\nEnter the interval : ";
    cin>>interval;

    ofstream excel;
    excel.open("test.csv");

    excel << "n, Time, Hour Angle, Declination, cosQ, cosQz, Ib, Id, Ig" << endl;

    for(date.tm_mon = 1; date.tm_mon<=12; date.tm_mon++)
    {
        int ctr=0;
        switch (date.tm_mon)
        {
        case 1:
            ctr=31;
            break;
        case 2:
            ctr=28;
            break;
        case 3:
            ctr=31;
            break;
        case 4:
            ctr=30;
            break;
        case 5:
            ctr=31;
            break;
        case 6:
            ctr=30;
            break;
        case 7:
            ctr=31;
            break;
        case 8:
            ctr=31;
            break;
        case 9:
            ctr=30;
            break;
        case 10:
            ctr=31;
            break;
        case 11:
            ctr=30;
            break;
        case 12:
            ctr=31;
            break;
        default:
            break;
        }

        date.tm_mday = 1;

        while (date.tm_mday<=ctr)
        {
            n = date.tm_mday + (date.tm_mon>1)*31 + (date.tm_mon>2)*28 + (date.tm_mon>3)*31 + (date.tm_mon>4)*30 + (date.tm_mon>5)*31 + (date.tm_mon>6)*30 + (date.tm_mon>7)*31 + (date.tm_mon>8)*31 + (date.tm_mon>9)*30 + (date.tm_mon>10)*31 + (date.tm_mon>11)*30;

            n = n + (1*(checkYear(date.tm_year) && date.tm_mon>2));     //if the year is leap year

            float deg = (360 * (284+n))/365;
            deg *= (3.14159/180);

            dec = 23.45 * sin(deg);
            dec *= 3.14159/180;            

            for(time.tm_hour = 5 ; time.tm_hour<18; time.tm_hour++)
            {
                for(time.tm_min = 0; time.tm_min<60; time.tm_min += interval)
                {        
                    hra = 15 * (12 - (time.tm_hour + time.tm_min/60));
                        
                    hra *= 3.14159/180;

                    cosQ = sin(lat)*(sin(dec)*cos(tilt) + cos(dec)*cos(azi)*cos(hra)*sin(tilt)) + cos(lat)*(cos(dec)*cos(hra)*cos(tilt) - sin(dec)*cos(azi)*sin(tilt)) + cos(dec)*sin(azi)*sin(hra)*sin(tilt);

                    cosQz = sin(lat)*sin(dec) + cos(lat)*cos(dec)*cos(hra);

                    float Ibn;

                    Ibn = val[(date.tm_mon-1)].A * exp(-val[(date.tm_mon-1)].B/cosQz);

                    Ib = Ibn * cosQz;

                    Id = val[(date.tm_mon-1)].C * Ibn;

                    Ig = Ib + Id;
                    hra = hra/(3.14159/180);
                    excel<<n<<" ,"<<time.tm_hour<<" : "<<time.tm_min<<" ,"<<hra<<" ,"<<dec<<" ,"<<cosQ<<" ,"<<cosQz<<" ,"<<Ib<<" ,"<<Id<<" ,"<<Ig<<endl;
                }
                
            }
    
        date.tm_mday++;
        }
    }
    excel.close();
}

int main()
{
float lat;      //latitude

time_date date, time;

cout<<"Enter the Year:	";
cin>>date.tm_year;

cout<<"\nEnter the Latitude in Degrees: ";
cin>>lat;

ashrae(date, time, lat);

return 0;
} 