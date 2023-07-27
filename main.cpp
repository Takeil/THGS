///CODE BY ME 0v0

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <dirent.h>

#define _XOPEN_SOURCE 700

#include "wtypes.h"

using namespace std;

struct Items
{
    int Food = 0; //0
    int Water = 0; //0
    int Medpack = 0; //0
    int Nightlock = 0; //0
    bool Backpack = false; //false
};

struct W
{
    //Info
    int NumOfMelee = 9;
    int NumOfRange = 5;

    //Melee
    bool Knife = false;
    bool Machete = false;
    bool Sword = false;
    bool Sickle = false;
    bool Mace = false;
    bool Axe = false;
    bool Dagger = false;
    bool Trident = false;
    bool Spear = false;

    //Ranged
    bool Bow = false;
    bool ThrKnife = false;
    bool ThrSpear = false;
    bool ThrAxe = false;
    bool ThrTrident = false;
};

struct MW
{
    int Knife = 10;
    int Machete = 15;
    int Sword = 20;
    int Sickle = 25;
    int Mace = 30;

    int Axe = 20;
    int Dagger = 10;
    int Trident = 25;
    int Spear = 20;
};

struct RW
{
    int Bow = 20;
    int ThrKnife = 20;

    int ThrSpear = 20;
    int ThrAxe = 20;
    int ThrTrident = 20;
};

struct Brain
{
    int WaterX = -1;
    int WaterY = -1;
};

void GetScreenRes(int&hor,int&ver)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop,&desktop);

    hor = desktop.right;
    ver = desktop.bottom;
}

void GenerateNewRandomMapPos(int&NewX,int&NewY, int&limit,int R)
{
GenerateRandomPositionAgain:
    NewX = rand()%limit+1;
    NewY = rand()%limit+1;

    int Center = limit/2;
    int DistanceOfPoints = sqrt(pow((NewX-Center),2)+pow((NewY-Center),2));

    if(DistanceOfPoints > Center)
        goto GenerateRandomPositionAgain;
}

void GenerateFoodPos(int&NewX,int&NewY, int limit)
{
GenerateRandomPositionAgain:
    NewX = rand()%limit+1;
    NewY = rand()%limit+1;

    int CenterMAX = limit/2;
    int CenterMIN = limit/4;
    int DistanceOfPointsMAX = sqrt(pow((NewX-CenterMAX),2)+pow((NewY-CenterMAX),2));
    int DistanceOfPointsMIN = sqrt(pow((NewX-CenterMAX),2)+pow((NewY-CenterMAX),2));

    if (DistanceOfPointsMAX >= CenterMAX || DistanceOfPointsMIN < CenterMIN)
        goto GenerateRandomPositionAgain;
}

void GenerateWeaponPos(int&X,int&Y,int limit)
{
GenerateRandomPositionAgain:
    X = rand()%limit+1;
    Y = rand()%limit+1;

    int R = limit/2;
    if (pow((X - R),2) + pow((Y - R),2) >= pow(R/15,2))
        goto GenerateRandomPositionAgain;
}

void GenerateFoodOnCornucopiaPos(int&X,int&Y,int limit)
{
GenerateRandomPositionAgain:
    X = rand()%limit+1;
    Y = rand()%limit+1;

    int R = limit/2;
    if (pow((X - R),2) + pow((Y - R),2) >= pow(R/10,2))
        goto GenerateRandomPositionAgain;
}

float areatri(int x1, int y1, int x2, int y2, int x3, int y3) //From geekforgeeks.com
{
    return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool isInside(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) //From geekforgeeks.com
{
    /* Calculate area of triangle ABC */
    float A = areatri (x1, y1, x2, y2, x3, y3);

    /* Calculate area of triangle PBC */
    float A1 = areatri (x, y, x2, y2, x3, y3);

    /* Calculate area of triangle PAC */
    float A2 = areatri (x1, y1, x, y, x3, y3);

    /* Calculate area of triangle PAB */
    float A3 = areatri (x1, y1, x2, y2, x, y);

    /* Check if sum of A1, A2 and A3 is same as A */
    return (A == A1 + A2 + A3);
}

void Wander(int&NewX,int X,int&NewY,int Y, int Radius, int ArenaSize)
{
RegenerateWanderPosition:
    NewX = rand()%(Radius+X)+1;
    NewY = rand()%(Radius+Y)+1;

    int DistanceOfPoints = 0;
    int Center = ArenaSize/2;
    DistanceOfPoints = sqrt(pow((NewX-Center),2)+pow((NewY-Center),2));

    if(DistanceOfPoints > Center)
        goto RegenerateWanderPosition;

    if (pow(NewX-X,2) + pow(NewY-Y,2) > pow(Radius,2))
        goto RegenerateWanderPosition;
}

void TrainingAreaWithCenter(int&NewX,int X,int&NewY,int Y, int Radius, int PX, int PY)
{
RegenerateWanderPosition:
    NewX = rand()%(Radius+X)+1;
    NewY = rand()%(Radius+Y)+1;

    int DistanceOfPoints = 0;
    DistanceOfPoints = sqrt(pow((NewX-PX),2)+pow((NewY-PY),2));

    if(DistanceOfPoints > PX+PY)
        goto RegenerateWanderPosition;

    if (pow(NewX-X,2) + pow(NewY-Y,2) > pow(Radius,2))
        goto RegenerateWanderPosition;
}

bool Walk(int NX, int&X, int NY, int&Y)
{
    if ((NX!=X)||(NY!=Y))
    {
        if (X!= NX)
        {
            if(X<NX)
                X++;
            else if (X>NX)
                X--;
        }
        if (Y!= NY)
        {
            if(Y<NY)
                Y++;
            else if (Y>NY)
                Y--;
        }
        return false;
    }
    else if ((NX==X)&&(NY==Y))
        return true;
}

void WanderSquare(int SX1, int SY1, int SX2, int SY2, int&X, int&Y)
{
    int W = abs(SX1 - SX2);
    int H = abs(SY1 - SY2);

    int sX = rand()%W+1;
    int sY = rand()%H+1;

    int SX=SX1;
    int SY=SY1;

    if (SX1 > SX2)
        SX = SX2;
    if (SY1 > SY2)
        SY = SY2;

    X = sX + SX;
    Y = sY + SY;
}

void CheckIfFoodInsideTree(int&NX,int&NY,int TX,int TY,int TR,int Screen)
{
CheckPositionAgain:
    if (pow((NX - TX),2) + pow((NY - TY),2) <= pow(TR,2))
    {
        GenerateFoodPos(NX,NY,Screen);
        goto CheckPositionAgain;
    }
}

void GenerateName(int Chars, string&Name, string&LName,int loop)
{
    string line;
    int MaleTotal = 0;
    int FemaleTotal = 0;
    int SurnameTotal = 0;

    ifstream male("data/Names/Male.txt");
    while(getline(male,line))
        MaleTotal++;

    ifstream female("data/Names/Female.txt");
    while(getline(female,line))
        FemaleTotal++;

    ifstream surname("data/Names/Surname.txt");
    while(getline(surname,line))
        SurnameTotal++;

    //Male
    if (loop%2 == 0)
    {
        int Lim = rand()%MaleTotal+1;
        ifstream file("data/Names/Male.txt");
        for (int i = 0; i <= Lim; i++)
        {
            getline(file,Name);
        }
    }

    //Female
    else
    {
        int Lim = rand()%FemaleTotal+1;
        ifstream file("data/Names/Female.txt");
        for (int i = 0; i <= Lim; i++)
        {
            getline(file,Name);
        }
    }

    //Surname
    int Lim2 = rand()%SurnameTotal+1;
    ifstream file2("data/Names/Surname.txt");
    for (int i = 0; i <= Lim2; i++)
    {
        getline(file2,LName);
    }
    //transform(LName.begin(), LName.end(), LName.begin(), ::toupper);
}

void PlaceNightlock(int X, int Y,int R)
{
    ALLEGRO_COLOR FOOD_COLOR = al_map_rgb(155,0,255);

    al_draw_filled_circle(X,Y,R/2,FOOD_COLOR);
}

void PlaceFood(int FoodSizeComparison, int&NumberOfFood,int X, int Y)
{
    ALLEGRO_COLOR FOOD_COLOR = al_map_rgb(255,0,155);

    for (int i = 0; i < NumberOfFood; i++)
        al_draw_filled_circle(X,Y,FoodSizeComparison/2,FOOD_COLOR);
}

void PlaceWeapons(int X, int Y, int R, int WeaponInfo)
{
    ALLEGRO_COLOR RANGE = al_map_rgb(205,102,0);
    ALLEGRO_COLOR MELEE = al_map_rgb(153,153,0);
    ALLEGRO_COLOR WEAPON_COLOR;

    switch (WeaponInfo)
    {
    case 1:
        WEAPON_COLOR = MELEE;
        break;
    case 2:
        WEAPON_COLOR = RANGE;
        break;
    }
    al_draw_filled_circle(X,Y,R,WEAPON_COLOR);
}

void PlaceBackpack(int X, int Y, int R)
{
    ALLEGRO_COLOR BACKPACK = al_map_rgb(0,100,100);

    al_draw_filled_circle(X,Y,R,BACKPACK);
}

void PlaceMedpack(int X, int Y, int R)
{
    ALLEGRO_COLOR MEDPACK = al_map_rgb(255,0,0);

    al_draw_filled_circle(X,Y,R,MEDPACK);
}

const char* ordinal_suffix(int n) //From stackoverflow by: einpoklum
{
    static const char suffixes [][3] = {"th", "st", "nd", "rd"};
    auto ord = n % 100;
    if (ord / 10 == 1)
    {
        ord = 0;
    }
    ord = ord % 10;
    if (ord > 3)
    {
        ord = 0;
    }
    return suffixes[ord];
}

void SetNewYear(int N) //from https://stackoverflow.com/ by Anton
{
    string strReplace = "Games_Simulated: " + to_string(N);
    string strNew = "Games_Simulated: " + to_string(N+1);
    ifstream filein("Tribute Files/Info.txt"); //File to read from
    ofstream fileout("Tribute Files/TempInfo.txt"); //Temporary file

    string line;
    string strTemp;
    int y =0;
    bool found = false;
    while(filein >> strTemp)
    {
        if (found)
        {
            strTemp = "";
            found = false;
        }
        if(strTemp + " "  + to_string(N) == strReplace)
        {
            strTemp = strNew;
            found = true;
        }
        fileout << strTemp;

        y++;
        if (y%2 != 0)
            fileout << " ";
        else
            fileout << "\n";
        //if(found) break;
    }
    filein.close();
    fileout.close();

    remove("Tribute Files/Info.txt");
    rename("Tribute Files/TempInfo.txt","Tribute Files/Info.txt");

}

int CheckInfoFile(int Line)
{
    ifstream File;

    File.open("Tribute Files/Info.txt");
    string Smthing;

    for (int i = 0; i <= Line; i++)
        getline(File,Smthing);

    File.close();

    stringstream str_strm;
    str_strm << Smthing; //convert the string s into stringstream
    string temp_str;
    int temp_int;
    while(!str_strm.eof())
    {
        str_strm >> temp_str; //take words into temp_str one by one
        stringstream(temp_str) >> temp_int;
        temp_str = ""; //clear temp string
    }

    return temp_int;
}

float area(int x1, int y1, int x2, int y2, int x3, int y3) //From geekforgeeks.com
{
    return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool check(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x, int y) //From geeksforgeeks.com
{
    float A = area(x1, y1, x2, y2, x3, y3) + area(x1, y1, x4, y4, x3, y3);
    float A1 = area(x, y, x1, y1, x2, y2);
    float A2 = area(x, y, x2, y2, x3, y3);
    float A3 = area(x, y, x3, y3, x4, y4);
    float A4 = area(x, y, x1, y1, x4, y4);
    return (A >= A1 + A2 + A3 + A4);
}

void SetStartPosition(int&X, int&Y, int Players, int Screen, int Radius, int increment, double ArenaAngle, double&Angle1)
{
    const double Circumference = 3.14159;
    double Angle = Circumference/Players;
    double proxy = Angle;

    for (int i = 0; i < increment; i++)
        Angle += proxy;

    Angle += ArenaAngle-Circumference-Circumference/2;

    X = Screen/2 + Radius * (cos(Angle));
    Y = Screen/2 + Radius * (sin(Angle));

    Angle1 = atan2((Screen/2)-Y,(Screen/2)-X) * 180 / Circumference;
    Angle1 = Angle1*(Circumference/180) - Circumference/4;
}

void SetTrainingPosition(int&X, int&Y, int Players, int Radius, int CenterX, int CenterY, int increment)
{
    const double C = 3.14159 * 2;
    double Angle = C/Players;
    double proxy = Angle;

    for (int i = 0; i < increment; i++)
        Angle += proxy;

    X = CenterX + Radius * (cos(Angle-1.57));
    Y = CenterY + Radius * (sin(Angle-1.57));
}

void SetStartMovement(int&X, int&Y, int Players, int Screen, int Radius, int increment, double ArenaAngle, double&Angle1)
{
    const double Circumference = 3.14159;
    double Angle = Circumference/Players;
    double proxy = Angle;

    for (int i = 0; i < increment; i++)
        Angle += proxy;

    Angle += ArenaAngle-Circumference-Circumference/2;

    X = Screen/2 + Radius*2 * (cos(Angle));
    Y = Screen/2 + Radius*2 * (sin(Angle));

    Angle1 = atan2((Screen/2)-Y,(Screen/2)-X) * 180 / Circumference;
    Angle1 = Angle1*(Circumference/180) - Circumference;
}

void GenerateEnvironmentTreesPos(int&X, int&Y, int&R, int MaxSize, int MinSize,int ScreenSize)
{
RegenerateTreeThickness:
    R = rand()%MaxSize+1;

    if (R < MinSize)
        goto RegenerateTreeThickness;

GenerateRandomPositionAgain:
    X = rand()%ScreenSize+1;
    Y = rand()%ScreenSize+1;

    int CenterMAX = ScreenSize/2;
    int CenterMIN = ScreenSize/4;
    int DistanceOfPointsMAX = sqrt(pow((X-CenterMAX),2)+pow((Y-CenterMAX),2));
    int DistanceOfPointsMIN = sqrt(pow((X-CenterMAX),2)+pow((Y-CenterMAX),2));

    if (DistanceOfPointsMAX >= CenterMAX || DistanceOfPointsMIN < CenterMIN)
        goto GenerateRandomPositionAgain;
}

void GeneratePondInfo(int&Num,int Max, int Min)
{
Regenerate:
    Num = rand()%Max+1;
    if (Num < Min)
        goto Regenerate;
}

void SetPondPosition(int&X, int&Y, int&R, int Max, int Min, int ScreenSize)
{
RegenerateTreeThickness:
    R = rand()%Max+1;

    if (R < Min)
        goto RegenerateTreeThickness;

GenerateRandomPositionAgain:
    X = rand()%ScreenSize+1;
    Y = rand()%ScreenSize+1;

    int CenterMAX = ScreenSize/2;
    int CenterMIN = ScreenSize/3;
    int DistanceOfPointsMAX = sqrt(pow((X-CenterMAX),2)+pow((Y-CenterMAX),2));
    int DistanceOfPointsMIN = sqrt(pow((X-CenterMAX),2)+pow((Y-CenterMAX),2));

    if (DistanceOfPointsMAX >= CenterMAX || DistanceOfPointsMIN < CenterMIN)
        goto GenerateRandomPositionAgain;
}

void SetPondPositionInsidePond(int&NewX,int X,int&NewY,int Y, int&Radius, int Max, int ArenaSize)
{
    Radius = rand()%Max+1;

RegenerateWanderPosition:
    NewX = rand()%(Radius+X)+1;
    NewY = rand()%(Radius+Y)+1;

    int DistanceOfPoints = 0;
    int Center = ArenaSize/2;
    DistanceOfPoints = sqrt(pow((NewX-Center),2)+pow((NewY-Center),2));

    if(DistanceOfPoints > Center)
        goto RegenerateWanderPosition;
    if (pow(NewX-X,2) + pow(NewY-Y,2) > pow(Radius,2))
        goto RegenerateWanderPosition;
}

bool CheckIfTreeIsOnPond(int PX, int PY, int PR, int EPX, int EPY, int EPR, int TX, int TY, int TR)
{
    if (pow((PX - TX),2) + pow((PY - TY),2) <= pow(PR,2))
        return true;
    if (pow((EPX - TX),2) + pow((EPY - TY),2) <= pow(EPR,2))
        return true;
    else
        return false;
}

bool CheckIfInsideCircle( int PX, int PY, int X, int Y, int R)
{
    if (pow((X - PX),2) + pow((Y - PY),2) <= pow(R,2))
        return true;
    else
        return false;
}

void CheckIfObjectIsInsideFire(int FX, int FY, int&OX, int&OY, int FR)
{
    if (pow((FX - OX),2) + pow((FY - OY),2) <= pow(FR,2))
    {
        OX = -999;
        OY = -999;
    }
}

void CheckIfObjectIsInsideWater(int WX, int WY, int WR, int EWX, int EWY, int EWR, int&OX, int&OY)
{
    if (pow((WX - OX),2) + pow((WY - OY),2) <= pow(WR/1.5,2))
    {
        OX = -999;
        OY = -999;
    }
    if (pow((EWX - OX),2) + pow((EWY - OY),2) <= pow(EWR/1.5,2))
    {
        OX = -999;
        OY = -999;
    }
}

void TurnEnvironmentTreesToSolidObjects(int&X, int&Y, int MidX, int MidY,int Radius, int Incrementation)
{
    X = round(MidX+Radius*cos(Incrementation));
    Y = round(MidY+Radius*sin(Incrementation));
}

void PrintEnvironmentTrees (int X, int Y, int Thickness)
{
    ALLEGRO_COLOR WOOD = al_map_rgb(139,69,19);

    al_draw_filled_circle(X,Y,Thickness,WOOD);
}

void PrintEnvironmentLeaves (int X, int Y, int Thickness)
{
    ALLEGRO_COLOR LEAF = al_map_rgb(0,102,0);

    al_draw_filled_circle(X,Y,Thickness,LEAF);
}

void PrintEnvironmentPond(int X, int Y, int Thickness)
{
    ALLEGRO_COLOR WATER = al_map_rgb(100,200,250);

    al_draw_filled_circle(X,Y,Thickness,WATER);
}

void CreateCornucopia(int&X, int&Y, int&X2, int&Y2, int&X3, int&Y3, int RC, int R, int Screen, double&Angle)
{
    double RandomPos = rand()%628;
    RandomPos/=100;

    X = Screen/2 + R * (cos(RandomPos));
    Y = Screen/2 + R * (sin(RandomPos));

    X2 = X + RC*.5 * (cos(RandomPos+1.5707));
    Y2 = Y + RC*.5 * (sin(RandomPos+1.5707));

    X2 = X2 + RC/1 * (cos(RandomPos+3.1415));
    Y2 = Y2 + RC/1 * (sin(RandomPos+3.1415));

    X3 = X + RC*.5 * (cos(RandomPos-1.5707));
    Y3 = Y + RC*.5 * (sin(RandomPos-1.5707));

    X3 = X3 + RC/1 * (cos(RandomPos-3.1415));
    Y3 = Y3 + RC/1 * (sin(RandomPos-3.1415));

    Angle = RandomPos;
}

void PrintCornucopia(int X, int Y, int X2, int Y2, int X3, int Y3, int R, ALLEGRO_COLOR BG, double Angle)
{
    ALLEGRO_COLOR Cornucopia = al_map_rgb(218,165,32);
    al_draw_filled_triangle(X,Y,X2,Y2,X3,Y3,Cornucopia);

    int EX = (X2 + X3) / 2;
    int WY = (Y2 + Y3) / 2;

    //al_draw_filled_circle(EX,WY,R/2.5,Cornucopia);
    al_draw_filled_circle(EX,WY,R/3,al_map_rgb(184,134,11));

    int EX2 = X2 + R/1.9 * (cos(Angle+3.1415));
    int WY2 = Y2 + R/1.9 * (sin(Angle+3.1415));

    int EX3 = X3 + R/1.9 * (cos(Angle-3.1415));
    int WY3 = Y3 + R/1.9 * (sin(Angle-3.1415));

    al_draw_line(EX3,WY3,EX2,WY2,BG,R);
}

int CountFiles(string FileDirectory)
{
    int num = 0;

    DIR *dp;
    struct dirent *ep;
    dp = opendir (FileDirectory.c_str());
    if (dp != NULL)
    {
        while (ep = readdir (dp))
            num++;//puts (ep->d_name);
        (void) closedir (dp);
    }
    else
        cout << "Couldn't open the directory: " << FileDirectory << endl;

    return num -= 2;
}

int main()
{
    srand(time(NULL));
    const int NumberOfPlayers = 24; // Default = 24; //Min = (Suggested) 2; Max = 48;

    int horizontal = 0;
    int vertical = 0;
    GetScreenRes(vertical,horizontal);
    int FPS = 30;

    int MainDisp = 533;
    int SecDispH = 362;
    int SecDispV = 266;
    int ThiDispH = 133;

    int SizeofChar = 2; //Default = MainDisp/ThiDispH; Suggested = 2;

    if(!al_init())
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, NULL);
        exit(0);
    }

    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_init_image_addon();

    al_reserve_samples(100);

    ALLEGRO_BITMAP * icon = al_load_bitmap("data/Images/Icon.png");
    ALLEGRO_DISPLAY * MainDisplay = al_create_display(MainDisp,MainDisp);
    al_set_display_icon(MainDisplay,icon);
    ALLEGRO_EVENT_QUEUE * Que = al_create_event_queue();
    ALLEGRO_EVENT_QUEUE * MouseQue = al_create_event_queue();
    ALLEGRO_EVENT Event;
    ALLEGRO_EVENT EventMouse;
    ALLEGRO_MOUSE_STATE state;
    ALLEGRO_TIMER * timer = NULL;

    ALLEGRO_COLOR BLACK = al_map_rgb(0,0,0);
    ALLEGRO_COLOR ARENA = al_map_rgb(0,20,0);
    ALLEGRO_COLOR WHITE = al_map_rgb(250,250,250);
    ALLEGRO_COLOR GREY = al_map_rgb(128,128,128);
    ALLEGRO_COLOR TRIBUTE_COLOR[NumberOfPlayers] = {};
    ALLEGRO_COLOR SKIN_COLOR[NumberOfPlayers] = {};
    ALLEGRO_COLOR HAIR_COLOR[NumberOfPlayers] = {};

    for (int i = 0; i < NumberOfPlayers; i++)
    {

        int r = rand()%210+46;
        int g = (r/9) * 7;
        int b = (r/9) * 6;

        SKIN_COLOR[i] = al_map_rgb(r,g,b);

        r = rand()%150 + 100;
        g = rand()%150 + 100;
        b = rand()%150 + 100;

        TRIBUTE_COLOR[i] = al_map_rgb(r,g,b);

        int Hairs = rand()%4+1;

        switch(Hairs)
        {
        case 1:
        {
            r = rand()%50+10;
            g = r;
            b = r;
        }
        break;

        case 2:
        {
            r = rand()%185+15;
            g = (r/15) * 7;
            b = (r/15) * 2;
        }
        break;

        case 3:
        {
            r = rand()%140+10;
            g = 0;
            b = 0;
        }
        break;

        case 4:
        {
            r = rand()%230+10;
            g = (r/24) * 23;
            b = (r/24) * 14;
        }
        break;
        }

        HAIR_COLOR[i] = al_map_rgb(r,g,b);
    }

    ALLEGRO_COLOR NAMECOLOR = al_map_rgb(0,0,0);

    int FontSize = (SecDispH/(NumberOfPlayers+1));

    if (FontSize > 100)
        FontSize = 20;

    ALLEGRO_FONT * NAME = al_load_font("data/Fonts/Hunger Games.ttf",FontSize*2,0);
    ALLEGRO_FONT * CAPITOL = al_load_font ("data/Fonts/BankGothic Bold.ttf",FontSize*1.2,0);
    ALLEGRO_FONT * DISTRICT = al_load_font("data/Fonts/OPTIBankGothic-Medium.otf",FontSize*3.5,0);
    ALLEGRO_FONT * ROBOTO_BOLD = al_load_font ("data/Fonts/RobotoMono-Bold.ttf",FontSize,0);
    ALLEGRO_FONT * ROBOTO_REGULAR = al_load_font("data/Fonts/RobotoMono-Regular.ttf",FontSize/1.5,0);
    ALLEGRO_FONT * DAYCOUNTER = al_load_font("data/Fonts/RobotoMono-Light.ttf",FontSize/2,0);

    ALLEGRO_FONT * REAPING_NAME = al_load_font ("data/Fonts/BankGothic Bold.ttf",FontSize,0);

    ALLEGRO_FONT * PREGAME_NAME = al_load_font("data/Fonts/OPTIBankGothic-Medium.otf",FontSize,0);
    ALLEGRO_FONT * PREGAME_DISTRICT = al_load_font ("data/Fonts/Compact Bold.ttf",FontSize*7,0);
    ALLEGRO_FONT * PREGAME_SCORE = al_load_font("data/Fonts/BankGothic Bold.ttf",FontSize*5,0);

    al_draw_text(CAPITOL,WHITE,MainDisp/2,MainDisp/2,ALLEGRO_ALIGN_CENTRE,"Loading...");
    al_flip_display();

    bool SoundEventPlayed = false;
    ALLEGRO_SAMPLE_ID SoundEvent;

    ///AMBIENT
    ALLEGRO_SAMPLE * Ambient = al_load_sample("data/Sound/Forest Ambient.ogg");

    ///FIRE
    ALLEGRO_SAMPLE * Fire_Start = al_load_sample("data/Sound/Fire - Start.ogg");
    ALLEGRO_SAMPLE * Fire_Loop = al_load_sample("data/Sound/Fire - Loop.ogg");
    ALLEGRO_SAMPLE * Fire_End = al_load_sample("data/Sound/Fire - End.ogg");

    ///WATER
    ALLEGRO_SAMPLE * Water_Start = al_load_sample("data/Sound/Water - Start.ogg");
    ALLEGRO_SAMPLE * Water_Loop = al_load_sample("data/Sound/Water - Loop.ogg");
    ALLEGRO_SAMPLE * Water_End = al_load_sample("data/Sound/Water - End.ogg");

    ///TRACKER JACKERS
    ALLEGRO_SAMPLE * Tracker_Jackers = al_load_sample("data/Sound/Tracker Jackers.ogg");

    ///GRUNTS, SCREAMS, DEATH SOUND
    bool EnableScream = CheckInfoFile(2);
    int Gender = 2;

    int DeathNum[Gender] = {};

    for (int i = 0; i < Gender; i++)
    {
        string Word;
        if (i == 0)
            Word = "Male";
        else if (i == 1)
            Word = "Female";

        DeathNum[i] = CountFiles("./data/Sound/Screams/" + Word +"/Death/");
    }

    ALLEGRO_SAMPLE * SoundDeath[Gender][100] = {};

    int GruntNumber = CountFiles("./data/Sound/Grunts");
    ALLEGRO_SAMPLE * TrainingGrunts[GruntNumber];

    for (int i = 0; i < GruntNumber; i++)
    {
        string meep = "data/Sound/Grunts/" + to_string(i+1) + ".ogg";
        TrainingGrunts[i] = al_load_sample(meep.c_str());
    }

    for (int i = 0; i < Gender; i++)
    {
        string meep;
        string Word;
        if (i == 0)
            Word = "Male";
        else if (i == 1)
            Word = "Female";

        for (int j = DeathNum[i]; j > 0; j--)
        {
            meep = "data/Sound/Screams/" + Word + "/Death/" + to_string(j) + ".ogg";
            SoundDeath[i][j] = al_load_sample(meep.c_str());
        }
    }

    ///SPONSOR
    ALLEGRO_SAMPLE * Sponsor = al_load_sample("data/Sound/Sponsor.ogg");

    ///BORDER
    ALLEGRO_SAMPLE * ArenaShrinkingound = al_load_sample("data/Sound/Arena Shrinking.ogg");

    ///BIRDS
    ALLEGRO_SAMPLE * Mockingjay = al_load_sample("data/Sound/Mockingjay Call.ogg");
    ///COUNTDOWN
    ALLEGRO_SAMPLE * Countdown = al_load_sample("data/Sound/Countdown.ogg");
    int CountdownLength = 15; ///Seconds

    ALLEGRO_SAMPLE * Cannon = al_load_sample("data/Sound/Cannon.ogg");
    int PlayedCannons = 0;
    int CannonDelay = 1*FPS;
    int CannonIncrement = 0;

    //ALLEGRO_SAMPLE * The_Fallen = al_load_sample("data/Sound/The Fallen.ogg");
    ALLEGRO_SAMPLE * The_Horn_Of_Plenty = al_load_sample("data/Sound/The Horn of Plenty.ogg");
    ALLEGRO_SAMPLE * Learning_The_Skills = al_load_sample("data/Sound/Learning the Skills.ogg");
    ALLEGRO_SAMPLE * Reaping_Day = al_load_sample("data/Sound/Reaping Day.ogg");
    ALLEGRO_SAMPLE * The_Hunger_Games = al_load_sample("data/Sound/The Hunger Games.ogg");
    ALLEGRO_SAMPLE * Skills_Reviews = al_load_sample("data/Sound/Skills Reviews.ogg");

    int SongLength = 100; ///Seconds
    bool Played = false;

    ALLEGRO_BITMAP * Capitol_Seal = al_load_bitmap("data/Images/The_Capitol_Seal.png");
    ALLEGRO_BITMAP * District_Seal = al_load_bitmap("data/Images/District.png");
    ALLEGRO_BITMAP * Training_Center = al_load_bitmap("data/Images/Training Center.jpg");
    ALLEGRO_BITMAP * The_Reaping = al_load_bitmap("data/Images/The Reaping.jpg");
    ALLEGRO_BITMAP * Logo1 = al_load_bitmap("data/Images/Logo 1.jpg");
    ALLEGRO_BITMAP * Logo2 = al_load_bitmap("data/Images/Logo 2.jpg");

    ALLEGRO_BITMAP * Head[NumberOfPlayers];
    ALLEGRO_BITMAP * Body[NumberOfPlayers];
    ALLEGRO_BITMAP * Hair[NumberOfPlayers];

    for (int i = 0; i < Gender; i++)
        for (int j = NumberOfPlayers-i; j >= 0; j-=2)
        {
            string Word;
            if (i == 1)
                Word = "Female";
            if (i == 0)
                Word = "Male";

            string meep = "data/Images/Portraits/" + Word + " Head.png";
            Head[j] = al_load_bitmap(meep.c_str());
            meep = "data/Images/Portraits/" + Word + " Shirt.png";
            Body[j] = al_load_bitmap(meep.c_str());

            if (i == 1)
                Word = "F";
            if (i == 0)
                Word = "M";

            int Hairs = CountFiles("./data/Images/Portraits/" + Word + " - Hair");

            meep = "data/Images/Portraits/" + Word + " - Hair/" + to_string(rand()%Hairs+1) + ".png";

            Hair[j] = al_load_bitmap(meep.c_str());
        }

    timer = al_create_timer(1.0/FPS);
    al_register_event_source(MouseQue,al_get_mouse_event_source());
    al_register_event_source(Que, al_get_timer_event_source(timer));

    al_set_window_position(MainDisplay,(((vertical/2)-((MainDisp+SecDispV)/2))+SecDispV)+7,(horizontal/2)-(MainDisp/1.7));
    al_set_window_title(MainDisplay, "Main Screen");

    ALLEGRO_DISPLAY * SecondDisplay = al_create_display(SecDispV,SecDispH);
    al_set_window_position(SecondDisplay,((vertical/2)-((MainDisp+SecDispV)/2))-7,(horizontal/2)-(MainDisp/1.7));
    al_set_window_title(SecondDisplay, "Second Screen");
    al_set_display_icon(SecondDisplay,icon);

    al_draw_text(CAPITOL,WHITE,SecDispV/2,SecDispH/2,ALLEGRO_ALIGN_CENTRE,"Loading...");
    al_flip_display();

    ALLEGRO_DISPLAY * ThirdDisplay = al_create_display(SecDispV,ThiDispH);
    al_set_window_position(ThirdDisplay,((vertical/2)-((MainDisp+SecDispV)/2))-7,((horizontal/2)+((SecDispH+ThiDispH)/2.26))-ThiDispH);
    al_set_window_title(ThirdDisplay, "Third Screen");
    al_set_display_icon(ThirdDisplay,icon);

    al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,"Loading...");
    al_flip_display();

    const float PI = 3.14159;
    const int AREA_RADIUS = SizeofChar*30;
    const float ANGLE_OFFSET = PI/2; // Default = PI; // 360 = 6.28; //FOV

    ///Names and last names
    string FirstName[NumberOfPlayers] = {};
    string LastName[NumberOfPlayers] = {};

    int GenderType[NumberOfPlayers] = {};

    ///Personalities
    char Energy[NumberOfPlayers] = {};          ///                                                     // I E
    char Information[NumberOfPlayers] = {};     ///Scan for enemy health    ///Scan for self needs      // N S
    char Decision[NumberOfPlayers] = {};        ///Essential in Training    ///Train or socialize more  // T F
    char Organization[NumberOfPlayers] = {};    ///Run to Cornucopia        ///Run away from cornucopia // J P

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (rand()%2 > 0)
            Energy[i] = 'E';
        else
            Energy[i] = 'I';

        if (rand()%2 > 0)
            Information[i] = 'N';
        else
            Information[i] = 'S';

        if (rand()%2 > 0)
            Decision[i] = 'T';
        else
            Decision[i] = 'F';

        if (rand()%2 > 0)
            Organization[i] = 'J';
        else
            Organization[i] = 'P';
    }

    string FullName[NumberOfPlayers] = {};

    bool Custom_Names = CheckInfoFile(5);

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        GenerateName(NumberOfPlayers,FirstName[i],LastName[i],i);

        if (i%2 != 0)
            GenderType[i] = 1;
        else
            GenderType[i] = 0;
    }

    if (Custom_Names)
    {
        string line;
        int N = 0;
        ifstream Names("Tribute Files/Custom Names.txt");

        if (Names.is_open())
        {
            while(getline(Names,line))
            {
                N++;
            }
            Names.close();
        }

        if (N == NumberOfPlayers*2)
        {
            int Number = 0;
            int Num = 0;
            int Num2 = 0;

            ifstream Names("Tribute Files/Custom Names.txt");
            while(getline(Names,line))
            {
                if (Number%2 == 0 && line != "Random;")
                {
                    FirstName[Num] = line;
                }
                else if (line != "Random;")
                {
                    LastName[Num2] = line;
                }
                if (Number% 2 == 0)
                    Num++;
                else
                    Num2++;
                Number++;
            }
        }
    }

    for (int i = 0 ; i < NumberOfPlayers; i++)
    FullName[i] = FirstName[i] + " " + LastName[i];

    //Position and New Position
    int PosX[NumberOfPlayers] = {};
    int PosY[NumberOfPlayers] = {};
    int NewPosX[NumberOfPlayers] = {};
    int NewPosY[NumberOfPlayers] = {};

    //Position Setters
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        PosX[i] = {MainDisp/2};
        PosY[i] = {MainDisp/2};
        NewPosX[i] = {MainDisp/2};
        NewPosY[i] = {MainDisp/2};
    }

    //Vision
    int AngleX[NumberOfPlayers] = {};
    int AngleY[NumberOfPlayers] = {};
    int AngleX2[NumberOfPlayers] = {};
    int AngleY2[NumberOfPlayers] = {};
    int AngleXX[NumberOfPlayers] = {};
    int AngleYY[NumberOfPlayers] = {};
    double Angle[NumberOfPlayers] = {};
    double NewAngle[NumberOfPlayers] = {};

    //Survival Needs
    int Status[NumberOfPlayers] = {};
    double FoodNeed[NumberOfPlayers] = {};
    double WaterNeed[NumberOfPlayers] = {};
    double Health[NumberOfPlayers] = {};
    int NumberOfFood = NumberOfPlayers/2;   //NumberOfPlayers; // Default: NumberOfPlayers
    int FoodPosX[NumberOfFood] = {};
    int FoodPosY[NumberOfFood] = {};
    int NumberOfNightlocks = rand()%3+1;
    int NightlockPosX[NumberOfNightlocks] = {};
    int NightlockPosY[NumberOfNightlocks] = {};
    Items Inventory[NumberOfPlayers] = {};
    Brain Memory[NumberOfPlayers] = {};

    //Declaration of Speed and Time
    const double SPEED = 1; // Default = 1;
    double Day = 0;

    //Deterioration Variables
    const int MaxOnDets = 100; //Default = 100;
    const double Deterioration = 0.05; // Default = 0.01;
    double HealthDeterioration[NumberOfPlayers] = {};
    double WaterDeteriorationRate[NumberOfPlayers] = {};
    double HungerDeteriorationRate[NumberOfPlayers] = {};
    double HealthRegeneration[NumberOfPlayers] = {};
    double DayIncrementation = (Deterioration * SPEED)/FPS;

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        HealthRegeneration[i] = Deterioration * SPEED + (SPEED/10);
        HealthDeterioration[i] = HealthRegeneration[i];
        WaterDeteriorationRate[i] = Deterioration * SPEED + (SPEED/100);
        HungerDeteriorationRate[i] = (Deterioration/7) * SPEED + (SPEED/100);
        FoodNeed[i] = MaxOnDets;
        WaterNeed[i] = MaxOnDets;
        Health[i] = MaxOnDets;
        Status[i] = 1;      //Default = 1;  // 1 = Alive; 0 = Dead;
    }

    //Create Food Positions
    int RandomFoodOnCornucopia = rand()%(NumberOfFood/2);
    for (int i = 0; i < NumberOfFood; i++)
        GenerateFoodPos(FoodPosX[i],FoodPosY[i],MainDisp);
    for (int i = 0; i < NumberOfNightlocks; i++)
        GenerateFoodPos(NightlockPosX[i],NightlockPosY[i],MainDisp);

    //Environment
    const int NUMBER_OF_TREES = NumberOfPlayers * 5;
    const int TREE_MAX_THICKNESS = SizeofChar*2;
    const int TREE_MIN_THICKNESS = SizeofChar;
    int TreeX[NUMBER_OF_TREES] = {};
    int TreeY[NUMBER_OF_TREES] = {};
    int TreeThickness[NUMBER_OF_TREES] = {};

    int NUMBER_OF_PONDS = 0;
    const int MAX_PONDS = 10;
    const int MIN_PONDS = 5;

    GeneratePondInfo(NUMBER_OF_PONDS,MAX_PONDS,MIN_PONDS);

    int PondSize[NUMBER_OF_PONDS] = {};
    const int MAX_POND_SIZE = 25;
    const int MIN_POND_SIZE = 5;

    for(int i = 0; i < NUMBER_OF_PONDS; i++)
        GeneratePondInfo(PondSize[i],MAX_POND_SIZE,MIN_POND_SIZE);

    int PondX[NUMBER_OF_PONDS] = {};
    int PondY[NUMBER_OF_PONDS] = {};
    int PondR[NUMBER_OF_PONDS] = {};

    for (int i = 0; i < NUMBER_OF_PONDS; i++)
        SetPondPosition(PondX[i],PondY[i],PondR[i],MAX_POND_SIZE,MIN_POND_SIZE,MainDisp);

    const int NUMBER_OF_PONDS_INSIDE_PONDS = 5;

    int ExtraPondX[NUMBER_OF_PONDS][NUMBER_OF_PONDS_INSIDE_PONDS] = {};
    int ExtraPondY[NUMBER_OF_PONDS][NUMBER_OF_PONDS_INSIDE_PONDS] = {};
    int ExtraPondR[NUMBER_OF_PONDS][NUMBER_OF_PONDS_INSIDE_PONDS] = {};

    for (int i = 0; i < NUMBER_OF_PONDS; i++)
        for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
            SetPondPositionInsidePond(ExtraPondX[i][j],PondX[i],ExtraPondY[i][j],PondY[i],ExtraPondR[i][j],MAX_POND_SIZE,MainDisp);

    //Create Tree Positions
    for(int i = 0; i < NUMBER_OF_TREES; i++)
    {
        GenerateEnvironmentTreesPos(TreeX[i],TreeY[i],TreeThickness[i],TREE_MAX_THICKNESS,TREE_MIN_THICKNESS, MainDisp);
    }

    for (int P = 0; P < 10; P++)
        for(int i = 0; i < NUMBER_OF_PONDS; i++)
            for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
                for (int k = 0; k < NUMBER_OF_TREES; k++)
                {
                    bool Check = CheckIfTreeIsOnPond(PondX[i],PondY[i],PondR[i],ExtraPondX[i][j],ExtraPondY[i][j],ExtraPondR[i][j],TreeX[k],TreeY[k],TreeThickness[k]);
                    while (Check)
                    {
                        Check = CheckIfTreeIsOnPond(PondX[i],PondY[i],PondR[i],ExtraPondX[i][j],ExtraPondY[i][j],ExtraPondR[i][j],TreeX[k],TreeY[k],TreeThickness[k]);
                        if (Check)
                        {
                            GenerateEnvironmentTreesPos(TreeX[k],TreeY[k],TreeThickness[k],TREE_MAX_THICKNESS,TREE_MIN_THICKNESS, MainDisp);
                        }
                    }
                }

    for (int P = 0; P < 2; P++)
        for(int i = 0; i < NUMBER_OF_PONDS; i++)
            for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
                for (int k = 0; k < NumberOfFood; k++)
                {
                    if (abs(sqrt(pow(FoodPosX[k]-ExtraPondX[i][j],2)+ pow(ExtraPondY[i][j]-FoodPosY[k],2))) <= ExtraPondR[i][j] ||
                            abs(sqrt(pow(FoodPosX[k]-PondX[i],2)+ pow(PondY[i]-FoodPosY[k],2))) <= PondR[i])
                    {
                        FoodPosX[k] = -999;
                        FoodPosY[k] = -999;
                    }
                }

    //Making Trees Solid Objects
    int TreeSolidationRotation = TreeThickness[0]*100;
    int TreeSolidX[NUMBER_OF_TREES][TreeSolidationRotation];
    int TreeSolidY[NUMBER_OF_TREES][TreeSolidationRotation];

    for (int i = 0; i < NUMBER_OF_TREES; i++)
    {
        for (int j = 0; j < TreeSolidationRotation; j++)
        {
            TurnEnvironmentTreesToSolidObjects(TreeSolidX[i][j],TreeSolidY[i][j],TreeX[i],TreeY[i],TreeThickness[i],j);
        }
    }

    const int LEAVES_MAX = TREE_MAX_THICKNESS*4;
    const int LEAVES_MIN = TREE_MIN_THICKNESS*2;
    int LeafR[NUMBER_OF_TREES] = {};
    int LeafX[NUMBER_OF_TREES] = {};
    int LeafY[NUMBER_OF_TREES] = {};

    for(int i = 0; i < NUMBER_OF_TREES; i++)
        GenerateEnvironmentTreesPos(LeafX[i],LeafY[i],LeafR[i],LEAVES_MAX,LEAVES_MIN,MainDisp);

    for(int i = 0; i < NUMBER_OF_TREES; i++)
    {
        LeafX[i] = TreeX[i];
        LeafY[i] = TreeY[i];
    }

    //Check if food is inside of a tree
    for (int i = 0; i < NUMBER_OF_TREES; i++)
        for (int j = 0; j < NumberOfFood; j++)
            CheckIfFoodInsideTree(FoodPosX[j],FoodPosY[j],TreeX[i],TreeY[i],TreeThickness[i],MainDisp);

    for (int i = 0; i < NUMBER_OF_TREES; i++)
        for (int j = 0; j < NumberOfNightlocks; j++)
            CheckIfFoodInsideTree(NightlockPosX[j],NightlockPosY[j],TreeX[i],TreeY[i],TreeThickness[i],MainDisp);

    int Rounded = round(MainDisp);
    int Grid[Rounded][Rounded] = {0};

    for (int i = 0; i < NUMBER_OF_TREES; i++)
    {
        for (int j = 0; j < TreeSolidationRotation; j++)
        {
            int ChangeX = TreeSolidX[i][j];
            int ChangeY = TreeSolidY[i][j];

            if (ChangeX >= MainDisp || ChangeY >= MainDisp || ChangeX <= 0 || ChangeY <= 0)
                break;
            else
                Grid[ChangeX][ChangeY] = 1;
        }
    }

    int ZeroValX[NumberOfPlayers] = {};
    int ZeroValY[NumberOfPlayers] = {};
    int MaxValX[NumberOfPlayers] = {};
    int MaxValY[NumberOfPlayers] = {};

    int CornucopiaX = 0;
    int CornucopiaY = 0;
    int CornucopiaX2 = 0;
    int CornucopiaY2 = 0;
    int CornucopiaX3 = 0;
    int CornucopiaY3 = 0;
    const int CornucopiaR = MainDisp/9;

    double CornucopiaAngle;
    CreateCornucopia(CornucopiaX,CornucopiaY,CornucopiaX2,CornucopiaY2,CornucopiaX3,CornucopiaY3,CornucopiaR,MainDisp/7,MainDisp,CornucopiaAngle);

    for (int i = 0; i < RandomFoodOnCornucopia; i++)
    {
Regenerate:
        GenerateFoodOnCornucopiaPos(FoodPosX[i],FoodPosY[i],MainDisp);
        if (isInside(CornucopiaX,CornucopiaY,CornucopiaX2,CornucopiaY2,CornucopiaX3,CornucopiaY3,FoodPosX[i],FoodPosY[i]))
            goto Regenerate;
    }

    const int NumberOfWeapons = NumberOfPlayers/2;
    int NumberOfMelee = NumberOfWeapons/2 + ((rand()%NumberOfWeapons/2));
    //int NumberOfRange = NumberOfWeapons - NumberOfMelee;

    MW MeleeWeapon = {};
    RW RangeWeapon = {};
    W EquipedWeapon[NumberOfPlayers] = {};

    int WeaponX[NumberOfWeapons] = {};
    int WeaponY[NumberOfWeapons] = {};
    int WeaponR[NumberOfWeapons] = {};

    int WeaponInfo[NumberOfWeapons] = {}; // 1 = Melee ;; 2 = Ranged
    int WeaponType[NumberOfWeapons] = {}; // 1 -> NumOfMelee;

    int Damage[NumberOfPlayers] = {};           ///MAX 10
    int CritChance[NumberOfPlayers] = {};       ///MAX 10
    int Dodgechance[NumberOfPlayers] = {};      ///MAX 10
    int SurvivalSkill[NumberOfPlayers] = {};    ///MAX 10

    int HasMWeapon[NumberOfWeapons] = {};
    int HasRWeapon[NumberOfWeapons] = {};

    //cout << NumberOfWeapons << " " << NumberOfMelee << " " << NumberOfRange << "\n";

    for (int i = 0; i < NumberOfWeapons; i++)
    {
        GenerateWeaponPos(WeaponX[i],WeaponY[i],MainDisp);
        WeaponR[i] = 1;
        if (i < NumberOfMelee)
        {
            WeaponType[i] = rand()%EquipedWeapon[i].NumOfMelee+1;
            WeaponInfo[i] = 1;
        }
        else
        {
            WeaponType[i] = rand()%EquipedWeapon[i].NumOfRange+1;
            WeaponInfo[i] = 2;
        }
    }
    for (int i = 0; i < NumberOfWeapons; i++)
    {
        for (int j = 0; j < NumberOfWeapons; j++)
        {
RecheckWeaponPos:
            if (WeaponX[i] == WeaponX[j] && WeaponY[i] == WeaponY[j] && i != j)
            {
                GenerateWeaponPos(WeaponX[i],WeaponY[i],MainDisp);
                goto RecheckWeaponPos;
            }
        }
    }

    int NumberOfBackpacks = NumberOfPlayers/4;
    int BackpackX[NumberOfBackpacks] = {};
    int BackpackY[NumberOfBackpacks] = {};
    int BackpackR = SizeofChar/2;

    for(int i = 0; i < NumberOfBackpacks; i++)
        GenerateWeaponPos(BackpackX[i],BackpackY[i],MainDisp);

    for (int i = 0; i < NumberOfBackpacks; i++)
    {
        for (int j = 0; j < NumberOfBackpacks; j++)
        {
RecheckBackpackPos:
            if (BackpackX[i] == BackpackX[j] && BackpackY[i] == BackpackY[j] && i != j)
            {
                GenerateWeaponPos(BackpackX[i],BackpackY[i],MainDisp);
                goto RecheckBackpackPos;
            }
        }
    }

    int NumberOfMedpacks = NumberOfPlayers/4;
    int MedpackPosX[NumberOfMedpacks] = {};
    int MedpackPosY[NumberOfMedpacks] = {};
    int MedpackPosR = SizeofChar/2;

    for (int i = 0; i < NumberOfMedpacks; i++)
        GenerateFoodPos(MedpackPosX[i],MedpackPosY[i],MainDisp);
    for(int i = 0; i < (rand()%NumberOfMedpacks); i++)
        GenerateWeaponPos(MedpackPosX[i],MedpackPosY[i],MainDisp);

    int AttackDelay[NumberOfPlayers] = {};
    int Attack[NumberOfPlayers] = {};

    for (int i = 0; i < NumberOfPlayers; i++)
        AttackDelay[i] = FPS/2; //Frames

    double RelationsDecrementation[NumberOfPlayers] = {};
    double Relations[NumberOfPlayers][NumberOfPlayers] = {};

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        if (Energy[i] == 'E')
            RelationsDecrementation[i] = 0.05;
        else
            RelationsDecrementation[i] = 0.02;
    }

    int PadestalX[NumberOfPlayers] = {};
    int PadestalY[NumberOfPlayers] = {};
    double PadestalR = SizeofChar+1;
    ALLEGRO_COLOR Padestal = al_map_rgb(192,192,192);

    int DeathOrder[NumberOfPlayers] = {};
    int DeathNumber = 1;
    string DeathText[NumberOfPlayers] = {};
    int KillCount[NumberOfPlayers] = {};

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        double Q;
        SetStartPosition(PadestalX[i],PadestalY[i],NumberOfPlayers,MainDisp,MainDisp/5.5,i,CornucopiaAngle,Q);
    }

    string EventText = "Event: None";
    bool ArenaEvent = false;
    bool Feasted = false;
    bool Counted = false;
    double EventTime = 1;
    int CountedPlayers = 0;

    ///WOLF MUTTS
    ///MONKEY MUTTS
    ///SQUIRREL MUTTS
    ///CANDY-PINK BIRDS
    ALLEGRO_COLOR Wolf      = al_map_rgb(105,105,105);
    ALLEGRO_COLOR Monkey    = al_map_rgb(255,51,51);
    ALLEGRO_COLOR Squirrel  = al_map_rgb(255,153,51);
    ALLEGRO_COLOR Birds     = al_map_rgb(255,51,153);
    ALLEGRO_SAMPLE * MuttSpawnSound;

    ALLEGRO_COLOR MuttColor;

    int MuttSight = AREA_RADIUS;
    int MuttSpeed = 1;
    int MuttDamage = 5;

    int NumberOfMutts = rand()%5 + 5;

    ///MUTT TYPE
    int MuttType = rand()%4;
    switch (MuttType)
    {
    case 0:
        MuttColor = Wolf;
        MuttSpawnSound = al_load_sample("data/Sound/Wolf Spawn.ogg");
        break;
    case 1:
        MuttColor = Monkey;
        MuttSpawnSound = al_load_sample("data/Sound/Monkey Spawn.ogg");
        break;
    case 2:
        MuttSight = MuttSight/2;
        MuttDamage = 3;
        NumberOfMutts += 5;
        MuttColor = Squirrel;
        MuttSpawnSound = al_load_sample("data/Sound/Squirrel Spawn.ogg");
        break;
    case 3:
        MuttSight = MuttSight*2;
        MuttDamage = 2;
        NumberOfMutts += 10;
        MuttColor = Birds;
        MuttSpawnSound = al_load_sample("data/Sound/Bird Spawn.ogg");
        break;
    }
    //al_play_sample(MuttSpawnSound,0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
    bool MuttsReleased = false;
    bool MuttsAlive = false;

    int MuttPosX[NumberOfMutts] = {};
    int MuttPosY[NumberOfMutts] = {};
    int MuttNewPosX[NumberOfMutts] = {};
    int MuttNewPosY[NumberOfMutts] = {};

    int MuttHealth[NumberOfMutts] = {};

    int MuttAlpha = rand()%NumberOfMutts;
    int MuttStatus[NumberOfMutts] = {};
    int MuttAttackDelay[NumberOfMutts] = {};
    int MuttAttack[NumberOfMutts] = {};

    for (int i = 0; i < NumberOfMutts; i++)
    {
        MuttStatus[i] = 1;
        MuttHealth[i] = 100;
        MuttAttackDelay[i] = FPS/2;
    }

    double MuttSpawn = (rand()%700);
    MuttSpawn/=100;

    int MuttHomeX = 0;
    int MuttHomeY = 0;

    for (int i = 0; i < NumberOfMutts; i++)
    {
        MuttPosX[i] = MainDisp/2 + MainDisp/2 * (cos(MuttSpawn));
        MuttPosY[i] = MainDisp/2 + MainDisp/2 * (sin(MuttSpawn));
        MuttNewPosX[i] = MuttPosX[i];
        MuttNewPosY[i] = MuttPosY[i];
        MuttHomeX = MuttPosX[i];
        MuttHomeY = MuttPosY[i];
    }

    ///TRACKER JACKERS
    ALLEGRO_COLOR TrackerJackers = al_map_rgb(184,134,11);

    int NumberOfTrackerJackerNests = rand()%3+1;
    int NumberOfTrackerJackers = 10; ///Per Nest

    int TrackerJackerNestX[NumberOfTrackerJackerNests] = {};
    int TrackerJackerNestY[NumberOfTrackerJackerNests] = {};
    int TrackerJackerNestR[NumberOfTrackerJackerNests] = {};

    int TrackerJackerNestDef = SizeofChar*5;
    int TrackerJackerDamage = 5;

    int TrackerJackerX[NumberOfTrackerJackerNests][NumberOfTrackerJackers] = {};
    int TrackerJackerY[NumberOfTrackerJackerNests][NumberOfTrackerJackers] = {};

    int TrackerJackerNewX[NumberOfTrackerJackerNests][NumberOfTrackerJackers] = {};
    int TrackerJackerNewY[NumberOfTrackerJackerNests][NumberOfTrackerJackers] = {};

    for (int i = 0; i < NumberOfTrackerJackerNests; i++)
    {
        int Tree = rand()%NUMBER_OF_TREES;
        TrackerJackerNestX[i] = TreeX[Tree] + ((rand()%10)-5);
        TrackerJackerNestY[i] = TreeY[Tree] + ((rand()%10)-5);
        TrackerJackerNestR[i] = TrackerJackerNestDef;

        for (int j = 0; j < NumberOfTrackerJackers; j++)
        {
            TrackerJackerX[i][j] = TrackerJackerNestX[i];
            TrackerJackerY[i][j] = TrackerJackerNestY[i];

            TrackerJackerNewX[i][j] = TrackerJackerX[i][j];
            TrackerJackerNewY[i][j] = TrackerJackerY[i][j];
        }
    }

    ///FIRE
    ALLEGRO_COLOR FIRE = al_map_rgba(255,153,51,0.5);

    int NumberOfFire = rand()%3+1;
    int FireX[NumberOfFire] = {};
    int FireY[NumberOfFire] = {};
    int FireR = 0;
    int FireRMax = MainDisp/3.5;

    bool Fired = false;
    bool Firing = false;

    int FireDamage = 1;

    int FireOnTribute[NumberOfPlayers] = {};
    int FireDuration = 1*FPS;       ///Time of damage when outside of fire

    bool FireBurning[NumberOfPlayers] = {};   ///Player/Tribute is still burning
    for (int i = 0; i < NumberOfPlayers; i++)
        FireBurning[i] = false;

    for (int i = 0; i < NumberOfFire; i++)
    {
        double FireSpawn = (rand()%700);
        FireSpawn/=100;

        FireX[i] = MainDisp/2 + MainDisp/2 * (cos(FireSpawn));
        FireY[i] = MainDisp/2 + MainDisp/2 * (sin(FireSpawn));
    }

    ///FLOOD
    int FloodMax = MainDisp/4;
    bool Flooded = false;
    bool Flooding = false;

    int FloodSize = 0;

    ///ARENA SHRINK
    int ArSi = 0;
    bool ArenaShrinking = false;
    bool ArenaShrunk = false;
    double BorderDamage = 5;

    int AlivePlayers = NumberOfPlayers;
    int DeadPlayers = 0;

    bool EnableSaveFile = CheckInfoFile(0);
    int Number = CheckInfoFile(1);
    string TITLE = "The " + to_string(Number) + ordinal_suffix(Number) + " Hunger Games";

    string Dist[NumberOfPlayers] = {};

    for (int i = 0 ; i < NumberOfPlayers; i++)
        Dist[i] = "District " + to_string((i/2)+1);

    string Count[NumberOfPlayers] = {};
    string Placement[NumberOfPlayers] = {};

    int Seconds = 0;
    int Proxy;
    bool PlayedAmbient = false;
    bool CountDown = true;
    bool CountedDown = false;
    bool Capitol_Seal_Printed = false;
    bool Printed = false;
    bool GameSimulation = true;
    bool PostGameStatistics = false;
    bool TheFallenDone = false;
    bool Paused = true;
    bool Paused2 = true;
    bool PlayedCountdown = false;

    double Down2 = 1;
    int Side = 0;
    bool running = true;

    bool PreGame = CheckInfoFile(4);
    int ScoringLength = 50;
    bool Scoring = false;
    bool Training = true;
    bool TrainingDeclaration = true;
    bool InTraining[NumberOfPlayers] = {};
    bool InQue[NumberOfPlayers] = {};
    int QueingIn[NumberOfPlayers] = {};
    int TrainingIn[NumberOfPlayers] = {};

    al_start_timer(timer);

    int SocialSpaceX[NumberOfPlayers] = {};
    int SocialSpaceY[NumberOfPlayers] = {};

    ///TRAINING VARIABLES
    int TrainingRangeQueLength = 4;
    int TrainingRangeOccupation = 2;
    int Training_Range_Que_X[TrainingRangeQueLength] = {};
    int Training_Range_Que_Y[TrainingRangeQueLength] = {};
    int Training_Range_X[TrainingRangeOccupation] = {};
    int Training_Range_Y[TrainingRangeOccupation] = {};

    int TrainingCriticalNum = 2;
    int TrainingCriticalCircles = 2;
    int TrainingCriticalQueLength = 3;
    int TrainingCriticalOccupation = 3;
    int Training_Critical_Que_X[TrainingCriticalNum][TrainingCriticalQueLength] = {};
    int Training_Critical_Que_Y[TrainingCriticalNum][TrainingCriticalQueLength] = {};
    int Training_Critical_X[TrainingCriticalNum][TrainingCriticalCircles] = {};
    int Training_Critical_Y[TrainingCriticalNum][TrainingCriticalCircles] = {};
    int Training_Critical_R[TrainingCriticalNum][TrainingCriticalCircles] = {};

    int TrainingMeleeQueLength = 5;
    int TrainingMeleeOccupation = 3;
    int Training_Melee_Que_X[TrainingMeleeQueLength] = {};
    int Training_Melee_Que_Y[TrainingMeleeQueLength] = {};
    int Training_Melee_X1 = 0;
    int Training_Melee_Y1 = 0;
    int Training_Melee_X2 = 0;
    int Training_Melee_Y2 = 0;

    int TrainingMAQue = 4;
    int TrainingMA_Que_X[TrainingMAQue] = {};
    int TrainingMA_Que_Y[TrainingMAQue] = {};
    int TrainingMA_X1 = 0;
    int TrainingMA_Y1 = 0;
    int TrainingMA_X2 = 0;
    int TrainingMA_Y2 = 0;

    int TrainingAgilityNum = 2;
    int TrainingAgilityQueLength = 3;
    int TrainingAgilityEdges[TrainingAgilityNum] = {};
    TrainingAgilityEdges[0] = 4;
    TrainingAgilityEdges[1] = 12;
    int Training_Agility_Que_X[TrainingAgilityNum][TrainingAgilityQueLength] = {};
    int Training_Agility_Que_Y[TrainingAgilityNum][TrainingAgilityQueLength] = {};
    int Training_Agility_X[TrainingAgilityNum][TrainingAgilityEdges[1]] = {};
    int Training_Agility_Y[TrainingAgilityNum][TrainingAgilityEdges[1]] = {};

    int TrainingSurvivalQue = 3;
    int TrainingSurvivalOccupation = 2;
    int Training_Survival_Que_X[TrainingSurvivalQue] = {};
    int Training_Survival_Que_Y[TrainingSurvivalQue] = {};
    int Training_Survival_X1 = 0;
    int Training_Survival_Y1 = 0;
    int Training_Survival_X2 = 0;
    int Training_Survival_Y2 = 0;

    int TargetX[NumberOfPlayers] = {};
    int TargetY[NumberOfPlayers] = {};
    bool WalkedToGoodPlace[NumberOfPlayers] = {};

    int Length_Of_Training[NumberOfPlayers] = {};

    int TributesTraining = true;

    int NumberOfGameMakers = 10;
    int GMPosX[NumberOfGameMakers] = {};
    int GMPosY[NumberOfGameMakers] = {};

    int Delay[NumberOfGameMakers] = {};
    int GMNewPosX[NumberOfGameMakers] = {};
    int GMNewPosY[NumberOfGameMakers] = {};

    ALLEGRO_COLOR GM_COLOR[NumberOfGameMakers] = {};

    for (int i = 0 ; i < NumberOfGameMakers; i++)
    {
        GMPosX[i] = MainDisp/10 + MainDisp/3 + SizeofChar*2 + 21;
        GMPosY[i] = MainDisp/10 + MainDisp/100 + 5;
        GMNewPosX[i] = MainDisp/10 + MainDisp/3 + SizeofChar*2 + 21;
        GMNewPosY[i] = MainDisp/10 + MainDisp/100 + 5;
        GM_COLOR[i] = al_map_rgb(rand()%155+100,rand()%155+100,rand()%155+100);
    }

    string TrainingText[NumberOfPlayers] = {};

    for (int i = 0; i < NumberOfPlayers; i++)
    {
        SurvivalSkill[i] = rand()%11;
        Damage[i] = rand()%11;
        CritChance[i] = rand()%11;
        Dodgechance[i] = rand()%11;
        for (int j = 0; j < NumberOfPlayers; j++)
        {
            Relations[i][j] = rand()%50;

            if ((i/2)+1 == (j/2)+1)
            {
                Relations[i][j] = rand()%100+100;
            }
        }
    }

    ///EASTER EGGS
    if (FullName[23] == "Katniss Everdeen")
    {
        Hair[23] = al_load_bitmap("data/Images/Portraits/F - Hair/2.png");
        HAIR_COLOR[23] = al_map_rgb(59,48,36);
        SKIN_COLOR[23] = al_map_rgb(178,135,100);
        TRIBUTE_COLOR[23] = al_map_rgb(34,139,34);
        SurvivalSkill[23] = 9;
        Damage[23] = 8;
        CritChance[23] = 9;
        Dodgechance[23] = 8;
        Relations[23][22] = 500000;
        Energy[23] = 'I';
        Information[23] = 'S';
        Decision[23] = 'T';
        Organization[23] =' J';
        al_play_sample(Mockingjay,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
    if (FullName[22] == "Peeta Mellark")
    {
        Hair[22] = al_load_bitmap("data/Images/Portraits/M - Hair/2.png");
        HAIR_COLOR[22] = al_map_rgb(215,190,167);
        SKIN_COLOR[22] = al_map_rgb(255,226,201);
        TRIBUTE_COLOR[22] = al_map_rgb(255,165,0);
        SurvivalSkill[22] = 5;
        Damage[22] = 7;
        CritChance[22] = 6;
        Dodgechance[22] = 6;
        Relations[22][23] = 500000;
        Energy[22] = 'I';
        Information[22] = 'S';
        Decision[22] = 'F';
        Organization[22] =' P';
    }

    double temp_SS[NumberOfPlayers] = {};
    double temp_DM[NumberOfPlayers] = {};
    double temp_CC[NumberOfPlayers] = {};
    double temp_DC[NumberOfPlayers] = {};
    double temp_RS[NumberOfPlayers][NumberOfPlayers] = {};

    while(Paused)
    {
        al_stop_timer(timer);
        Paused = true;

        if (!Printed)
        {
            al_play_sample(The_Hunger_Games,1,0,1,ALLEGRO_PLAYMODE_LOOP,NULL);

            al_set_target_backbuffer(ThirdDisplay);
            al_clear_to_color(BLACK);

            al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,"[press anywhere to start]");

            al_flip_display();

            al_set_target_backbuffer(SecondDisplay);
            al_clear_to_color(BLACK);

            al_draw_bitmap(Logo2,0,0,NULL);

            al_flip_display();

            al_set_target_backbuffer(MainDisplay);
            al_clear_to_color(BLACK);

            al_draw_bitmap(Logo1,0,0,NULL);

            al_flip_display();

            Printed = true;
        }

        al_register_event_source(MouseQue, al_get_display_event_source(MainDisplay));
        al_register_event_source(MouseQue, al_get_display_event_source(SecondDisplay));
        al_register_event_source(MouseQue, al_get_display_event_source(ThirdDisplay));
        //To Close
        al_wait_for_event(MouseQue, &EventMouse);
        if(EventMouse.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            exit(0);

        al_get_mouse_state(&state);
        if (state.buttons & 1)
            if(EventMouse.mouse.button)
                if (EventMouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if(EventMouse.mouse.button & 1)
                    {
                        al_stop_samples();
                        al_play_sample(Cannon,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        Printed = false;
                        Paused = false;
                        al_clear_to_color(BLACK);
                        al_start_timer(timer);
                    }
                }
    }

    bool Reaping = CheckInfoFile(3);
    bool Transitioning = CheckInfoFile(3);

    int ReapingDistrict = 0;
    int H = 20;
    int W = 10;
    int PN = H*W;
    int PX[2][PN] = {{}};
    int PY[2][PN] = {{}};
    ALLEGRO_COLOR PC[2][PN];

    int ReapingLength = 75;

    if(Reaping)
        al_play_sample(Reaping_Day,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

    while(Reaping)
    {
        int x = MainDisp/5;
        int y = MainDisp/7;

        Printed = false;
        Seconds++;

        for (int i = 0; i < NumberOfPlayers; i+=2)
        {
            if ((Seconds/FPS) == ReapingLength/24*(i+1) && !Printed && Seconds/FPS != Proxy)
            {
                al_set_target_backbuffer(SecondDisplay);
                al_clear_to_color(BLACK);

                al_draw_tinted_scaled_bitmap(District_Seal,al_map_rgb(100,100,100),0,0,500,589,(SecDispV/2)-125,(SecDispH/2)-147.25,250,294.5,NULL);
                al_draw_text(PREGAME_DISTRICT,al_map_rgb(100,100,100),SecDispV/2,(SecDispH/2)-FontSize*5.6,ALLEGRO_ALIGN_CENTRE,to_string((ReapingDistrict/2)+1).c_str());

                al_flip_display();

                al_set_target_backbuffer(ThirdDisplay);
                al_clear_to_color(BLACK);

                al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,"REAPING DAY");

                al_flip_display();

                al_set_target_backbuffer(MainDisplay);
                for (int i = 0; i < PN; i++)
                {
                    for (int j = 0; j < 2; j++)
                        PC[j][i] = al_map_rgb(200,200,200);
                }

                for (int k = 0; k < 2; k++)
                {
                    int r = 6;
                    int y = 0;
                    int X = rand()%W;
                    int Y = rand()%H;
                    int x;
                    int M;
                    int N = 0;

                    if (k == 0)
                        x = MainDisp/r;
                    else
                        x = MainDisp-MainDisp/r;

                    if (k == 0)
                        M = 1;
                    else
                        M = -1;

                    for (int i = 0; i < W; i++)
                    {
                        y = MainDisp/2;
                        x += (SizeofChar*6)*M;
                        for (int j = 0; j < H; j++)
                        {
                            y+= SizeofChar*6;

                            if (i==X && j==Y)
                            {
                                if (k == 0)
                                    PosX[k+ReapingDistrict] = MainDisp/2.2;
                                else
                                    PosX[k+ReapingDistrict] = MainDisp-MainDisp/2.2;

                                PosY[k+ReapingDistrict] = MainDisp/2.4;

                                PX[k][N] = MainDisp/2;
                                PY[k][N] = MainDisp/2.4;
                            }
                            else
                            {
                                PX[k][N] = x;
                                PY[k][N] = y;
                            }

                            N++;
                        }
                    }
                }

                al_set_target_backbuffer(MainDisplay);
                al_clear_to_color(BLACK);

                al_draw_bitmap(The_Reaping,0,0,NULL);

                for(int j = 0; j < 2; j++)
                    for (int i = 0; i < PN; i++)
                        al_draw_filled_circle(PX[j][i],PY[j][i],SizeofChar*1.5,PC[j][i]);

                for (int i = 0; i < 2; i++)
                {
                    int X = (MainDisp/3.7)-50;

                    if (i%2 != 0)
                        X = MainDisp-MainDisp/3.7-50;

                    int Y = (MainDisp/6)-50;

                    al_draw_tinted_scaled_bitmap(Head[i+ReapingDistrict],SKIN_COLOR[i+ReapingDistrict],0,0,500,500,X,Y,100,100,NULL);
                    al_draw_tinted_scaled_bitmap(Body[i+ReapingDistrict],PC[1][1],0,0,500,500,X,Y,100,100,NULL);
                    al_draw_tinted_scaled_bitmap(Hair[i+ReapingDistrict],HAIR_COLOR[i+ReapingDistrict],0,0,500,500,X,Y,100,100,NULL);
                    al_draw_text(REAPING_NAME,WHITE,X+50,Y+80,ALLEGRO_ALIGN_CENTRE,FullName[i+ReapingDistrict].c_str());

                    al_draw_filled_circle(PosX[i+ReapingDistrict],PosY[i+ReapingDistrict],SizeofChar*1.5,PC[1][1]);
                }
                al_flip_display();

                ReapingDistrict+=2;

                Printed = true;
                Proxy = Seconds/FPS;
            }
        }

        if (Seconds/FPS >= ReapingLength)
        {
            Seconds = 0;
            Reaping = false;
        }

        al_register_event_source(Que, al_get_display_event_source(MainDisplay));
        al_register_event_source(Que, al_get_display_event_source(SecondDisplay));
        al_register_event_source(Que, al_get_display_event_source(ThirdDisplay));

        al_wait_for_event(Que, &Event);
        if(Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            Reaping = false;
            PreGame = false;
            running = false;
            Transitioning = false;
        }
    }

    ///TRANSITION OF REAPING TO TRAINING
    ///20 SECONDS
    ///Ye

    int N = 0;
    while (Transitioning)
    {
        Seconds++;

        int TextX = FontSize;
        int TextY = 10;

        if (!Printed)
        {
            al_set_target_backbuffer(MainDisplay);
            al_clear_to_color(BLACK);

            al_draw_scaled_bitmap(Capitol_Seal,0,0,800,800,MainDisp/2-150,MainDisp/2-150,300,300,NULL);
            al_draw_text(CAPITOL,WHITE,MainDisp/2,MainDisp-MainDisp/5,ALLEGRO_ALIGN_CENTRE,"PANEM TODAY PANEM TOMORROW PANEM FOREVER");

            Printed = true;
            al_flip_display();
        }

        al_set_target_backbuffer(SecondDisplay);

        if (Seconds%20 == 0)
        {
            al_clear_to_color(BLACK);
            N++;

            if (N >= NumberOfPlayers)
            {
                N = NumberOfPlayers;
            }

            for (int i = 0; i < N; i++)
            {
                ALLEGRO_COLOR COLOR = al_map_rgb(200,200,200);
                if (N == NumberOfPlayers)
                    COLOR = TRIBUTE_COLOR[i];

                al_draw_filled_rectangle(TextX-FontSize/10,TextY,TextX+MaxOnDets+FontSize/10,TextY+FontSize*1.8,al_map_rgb(20,20,20));
                al_draw_text(ROBOTO_REGULAR,COLOR,TextX,TextY,ALLEGRO_ALIGN_LEFT,FullName[i].c_str());
                al_draw_text(ROBOTO_REGULAR,COLOR,TextX,TextY+FontSize,ALLEGRO_ALIGN_LEFT,"Waiting...");

                TextY = TextY + FontSize/2;
                TextY = TextY - FontSize/2.5;
                TextX = TextX + SecDispV/2;

                if (i%2 != 0)
                {
                    TextX = FontSize;
                    TextY = TextY + FontSize + FontSize/1.1;
                }
            }
        }


        al_flip_display();

        al_set_target_backbuffer(ThirdDisplay);
        al_clear_to_color(BLACK);
        al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2-FontSize/2,ALLEGRO_ALIGN_CENTRE,"LOADING");
        al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2+FontSize/2,ALLEGRO_ALIGN_CENTRE,"THE TRAINING CENTER");
        al_flip_display();

        if (Seconds >= 20*FPS)
        {
            Seconds = 0;
            Printed = false;
            Transitioning = false;
        }

        al_register_event_source(Que, al_get_display_event_source(MainDisplay));
        al_register_event_source(Que, al_get_display_event_source(SecondDisplay));
        al_register_event_source(Que, al_get_display_event_source(ThirdDisplay));

        al_wait_for_event(Que, &Event);
        if(Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            PreGame = false;
            running = false;
            Transitioning = false;
        }
    }


    for(int i = 0; i < NumberOfPlayers; i++)
    {
        SetTrainingPosition(SocialSpaceX[i],SocialSpaceY[i],NumberOfPlayers,MainDisp/15,MainDisp/1.69,MainDisp/2.29,i);
        SetTrainingPosition(PosX[i],PosY[i],NumberOfPlayers,MainDisp/15,MainDisp/1.69,MainDisp/2.29,i);
        SetTrainingPosition(NewPosX[i],NewPosY[i],NumberOfPlayers,MainDisp/15,MainDisp/1.69,MainDisp/2.29,i);
    }

    if(PreGame)
        al_play_sample(Learning_The_Skills,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
    while(PreGame)
    {
        al_set_target_backbuffer(MainDisplay);
        int x = 0;
        int y = 0;
        int x2 = 0;
        int y2 = 0;

        if (TrainingDeclaration)
        {

            al_set_target_backbuffer(MainDisplay);
            al_clear_to_color(BLACK);

            al_draw_filled_rectangle(MainDisp/10
                                     ,MainDisp/10
                                     ,MainDisp-MainDisp/10
                                     ,MainDisp-MainDisp/10
                                     ,al_map_rgb(20,20,20));

            ///GAME MAKERS
            al_draw_filled_rectangle(MainDisp/10 + MainDisp/3 + SizeofChar*2,           MainDisp/10 + MainDisp/100,
                                     MainDisp-MainDisp/10 - MainDisp/6 - SizeofChar*2,  MainDisp/10 + MainDisp/5,
                                     WHITE);

            ///RANGE
            x = MainDisp/10 + MainDisp/100;
            y = MainDisp/10 + MainDisp/100;
            x2 = MainDisp/10 + MainDisp/3;
            y2 = MainDisp/10 + MainDisp/5;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingRangeQueLength; i++)
            {
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Range_Que_X[i] = x + (X - X/10);
                Training_Range_Que_Y[i] = y + (Y*(i+1)/(TrainingRangeQueLength+1));

                al_draw_filled_circle(Training_Range_Que_X[i],Training_Range_Que_Y[i],3,GREY);
            }

            for (int i = 0; i < TrainingRangeOccupation; i++)
            {
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Range_X[i] = x + (X - X/4);
                Training_Range_Y[i] = y + (Y*(i+1)/(TrainingRangeOccupation+1));

                al_draw_filled_circle(Training_Range_X[i],Training_Range_Y[i],3,GREY);
            }

            ///CRIICAL
            x = MainDisp-MainDisp/10 - MainDisp/100;
            y = MainDisp/10 + MainDisp/100;
            x2 = MainDisp-MainDisp/10 - MainDisp/6;
            y2 = MainDisp/10 + MainDisp/5;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingCriticalQueLength; i++)
            {
                int N = 0;
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Critical_Que_X[N][i] = x - (X*(i+1)/(TrainingCriticalQueLength+1));
                Training_Critical_Que_Y[N][i] = y + (Y - Y/10);

                Training_Critical_X[N][N] = x - X/2;
                Training_Critical_Y[N][N] = y + Y/2.5;
                Training_Critical_R[N][N] = X/2.5;

                al_draw_filled_circle(Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N],GREY);

                al_draw_filled_circle(Training_Critical_Que_X[N][i],Training_Critical_Que_Y[N][i],3,GREY);
            }

            ///CRITICAL 2
            x = MainDisp/10 + MainDisp/100;
            y = MainDisp/10 + MainDisp/5 + SizeofChar*2;
            x2 = MainDisp/10 + MainDisp/7;
            y2 = MainDisp-MainDisp/10 - MainDisp/5 - SizeofChar*2;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingCriticalQueLength; i++)
            {
                int N = 1;
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Critical_Que_X[N][i] = x + (X - X/10);
                Training_Critical_Que_Y[N][i] = y + (Y*(i+1)/(TrainingCriticalQueLength+1));

                Training_Critical_X[N][N] = x + X/2.5;
                Training_Critical_Y[N][N] = y + Y/2;
                Training_Critical_R[N][N] = X/3;

                al_draw_filled_circle(Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N],GREY);
                al_draw_filled_circle(Training_Critical_Que_X[N][i],Training_Critical_Que_Y[N][i],3,GREY);
            }

            ///MELEE
            x = MainDisp/10 + MainDisp/100;
            y = MainDisp-MainDisp/10 - MainDisp/100;
            x2 = MainDisp/10 + MainDisp/3;
            y2= MainDisp-MainDisp/10 - MainDisp/5;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0 ; i < TrainingMeleeQueLength; i++)
            {
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Melee_Que_X[i] = x + (X - X/10);
                Training_Melee_Que_Y[i] = y - (Y*(i+1)/(TrainingMeleeQueLength+1));

                Training_Melee_X1 = x + X/10;
                Training_Melee_Y1 = y - Y/10;
                Training_Melee_X2 = x + (X-X/5);
                Training_Melee_Y2 = y - (Y-Y/10);

                al_draw_filled_rectangle(Training_Melee_X1,Training_Melee_Y1,Training_Melee_X2,Training_Melee_Y2,GREY);

                al_draw_filled_circle(Training_Melee_Que_X[i],Training_Melee_Que_Y[i],3,GREY);
            }

            ///MELEE + AGILITY
            x = MainDisp/10 + MainDisp/7 + SizeofChar*2;
            y = MainDisp/10 + MainDisp/3 + SizeofChar*2;
            x2 = MainDisp/10 + MainDisp/3.5;
            y2 = MainDisp-MainDisp/10 - MainDisp/5 - SizeofChar*2;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingMAQue; i++)
            {
                int X = abs(x-x2);
                int Y = abs(y-y2);

                TrainingMA_Que_X[i] = x + (X*(i+1)/(TrainingMAQue+1));
                TrainingMA_Que_Y[i] = y + (Y/10);

                al_draw_filled_circle(TrainingMA_Que_X[i],TrainingMA_Que_Y[i],3,GREY);

                TrainingMA_X1 = x + (X/2);
                TrainingMA_Y1 = y + (Y/5);
                TrainingMA_X2 = x + (X/2);
                TrainingMA_Y2 = y + (Y-Y/5);

                al_draw_line(TrainingMA_X1,TrainingMA_Y1,TrainingMA_X2,TrainingMA_Y2,GREY,3);
            }

            ///AGILITY
            x = MainDisp-MainDisp/10 - MainDisp/6 - SizeofChar*2;
            y = MainDisp-MainDisp/10 - MainDisp/100;
            x2 = MainDisp-MainDisp/10 - MainDisp/3.2 + SizeofChar;
            y2 = MainDisp-MainDisp/10 - MainDisp/3;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingAgilityQueLength; i++)
            {
                int N = 0;
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Agility_Que_X[N][i] = x - (X*(i+1)/(TrainingAgilityQueLength+1));
                Training_Agility_Que_Y[N][i] = y - (Y-Y/10);

                for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                {
                    int K;

                    if (j < TrainingAgilityEdges[N]/2)
                    {
                        Training_Agility_X[N][j] = x - (X/4);
                        Training_Agility_Y[N][j] = y2 + y2/7 + (Y*(j*2)/((TrainingAgilityEdges[N]/2)+1));
                        K = j;
                    }

                    if (j >= TrainingAgilityEdges[N]/2)
                    {
                        Training_Agility_X[N][j] = x - (X-X/4);
                        Training_Agility_Y[N][j] = Training_Agility_Y[N][K-(j-K-1)];
                    }

                    al_draw_filled_circle(Training_Agility_X[N][j],Training_Agility_Y[N][j],3,GREY);
                }

                for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                    if (j != TrainingAgilityEdges[N]-1)
                        al_draw_line(Training_Agility_X[N][j],Training_Agility_Y[N][j],Training_Agility_X[N][j+1],Training_Agility_Y[N][j+1],GREY,2);

                al_draw_filled_circle(Training_Agility_Que_X[N][i],Training_Agility_Que_Y[N][i],3,GREY);
            }

            ///AGILITY 2
            x = MainDisp/10 + MainDisp/3 + SizeofChar*2;
            y = MainDisp-MainDisp/10 - MainDisp/100;
            x2 = MainDisp-MainDisp/10 - MainDisp/3.2 - SizeofChar;
            y2 = MainDisp-MainDisp/10 - MainDisp/3;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingAgilityQueLength; i++)
            {
                int N = 1;
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Agility_Que_X[N][i] = x + (X*(i+1)/(TrainingAgilityQueLength+1));
                Training_Agility_Que_Y[N][i] = y - (Y-Y/10);

                for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                {
                    SetTrainingPosition(Training_Agility_X[N][j],Training_Agility_Y[N][j],TrainingAgilityEdges[N],30,x+X/2,y-Y/2,j);
                    al_draw_filled_circle(Training_Agility_X[N][j],Training_Agility_Y[N][j],3,GREY);
                }

                for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                    if (j != TrainingAgilityEdges[N]-1)
                        al_draw_line(Training_Agility_X[N][j],Training_Agility_Y[N][j],Training_Agility_X[N][j+1],Training_Agility_Y[N][j+1],GREY,2);

                al_draw_filled_circle(Training_Agility_Que_X[N][i],Training_Agility_Que_Y[N][i],3,GREY);
            }


            ///SURVIVAL SKILL
            x = MainDisp-MainDisp/10 - MainDisp/100;
            y = MainDisp-MainDisp/10 - MainDisp/100;
            x2 = MainDisp-MainDisp/10 - MainDisp/10;
            y2 = MainDisp/10 + MainDisp/3 + SizeofChar*2;
            //al_draw_filled_rectangle(x,y,x2,y2,WHITE);

            for (int i = 0; i < TrainingSurvivalQue; i++)
            {
                int X = abs(x-x2);
                int Y = abs(y-y2);

                Training_Survival_Que_X[i] = x - (X-X/5);
                Training_Survival_Que_Y[i] = y - (Y*(i+1)/(TrainingSurvivalQue+1));

                Training_Survival_X1 = x - (X/5);
                Training_Survival_Y1 = y - (Y/10);
                Training_Survival_X2 = x - (X-X/3);
                Training_Survival_Y2 = y - (Y-Y/10);

                al_draw_filled_circle(Training_Survival_Que_X[i],Training_Survival_Que_Y[i],3,GREY);
                al_draw_filled_rectangle(Training_Survival_X1,Training_Survival_Y1,Training_Survival_X2,Training_Survival_Y2,GREY);
            }

            ////CIRCLE
            //al_draw_filled_circle(MainDisp/1.7,MainDisp/2.3,MainDisp/11,al_map_rgb(40,40,40));
            //al_draw_filled_circle(MainDisp/2,MainDisp/2,SizeofChar,WHITE);

            al_draw_filled_circle(floor(MainDisp/3.2),floor(MainDisp/2.7),3,GREY);
            al_draw_filled_circle(floor(MainDisp-MainDisp/4),floor(MainDisp/2.7),3,GREY);
            al_draw_filled_circle(floor(MainDisp/2.4),floor(MainDisp-MainDisp/2.7),3,GREY);
            al_draw_filled_circle(floor(MainDisp-MainDisp/4),floor(MainDisp-MainDisp/2.7),3,GREY);

            al_draw_filled_circle(floor(MainDisp/2.5),floor(MainDisp/3),3,GREY);

            TrainingDeclaration = false;
        }

        if (Training)
        {
            al_clear_to_color(BLACK);
            Scoring = true;
            al_draw_bitmap(Training_Center,0,0,NULL);

            for (int i = 0; i < NumberOfGameMakers; i++)
            {
                al_draw_filled_circle(GMPosX[i],GMPosY[i],SizeofChar*1.5,GM_COLOR[i]);
                int D = rand()%300;

                if (GMNewPosX[i] == GMPosX[i] && GMNewPosY[i] == GMPosY[i] && Delay[i] <= 0)
                {
                    WanderSquare(MainDisp/10 + MainDisp/3 + SizeofChar*2 + 5, MainDisp/10 + MainDisp/100 + 5, MainDisp-MainDisp/10 - MainDisp/6 - SizeofChar*2 - 5, MainDisp/10 + MainDisp/7 - 5, GMNewPosX[i],GMNewPosY[i]);
                    Delay[i] = D;
                }
                else
                    Delay[i]--;

                if (Day >= 3)
                {
                    x = MainDisp/10 + MainDisp/3 + SizeofChar*2 + 5;
                    y = MainDisp/10 + MainDisp/100 + 5;
                    x2 = MainDisp-MainDisp/10 - MainDisp/6 - SizeofChar*2 - 5;
                    y2 = MainDisp/10 + MainDisp/7 - 5;

                    int X = abs(x-x2);
                    int Y = abs(y-y2);

                    GMNewPosX[i] = x + (X*(i+1)/(NumberOfGameMakers+1));
                    GMNewPosY[i] = y + (Y - Y/10);
                }

                Walk(GMNewPosX[i],GMPosX[i],GMNewPosY[i],GMPosY[i]);
            }


            if (TributesTraining)
            {
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    int TRDONE = false;

                    if(PosX[i] == SocialSpaceX[i] && PosY[i] == SocialSpaceY[i])
                    {
                        TrainingText[i] = "Socializing...";
                        InTraining[i] = false;
                        WalkedToGoodPlace[i] = false;
                        TargetX[i] = PosX[i];
                        TargetY[i] = PosY[i];
                    }

                    if (!InQue[i] && (rand()%100 <= 0 || Decision[i] == 'T'))
                    {
                        if (!InTraining[i])
                        {
                            Length_Of_Training[i] = rand()%151+150;
                            int TrainingArea = rand()%8+1;

                            switch(TrainingArea)
                            {
                            case 1:
                            {
                                for (int j = 0; j < TrainingRangeQueLength; j++)
                                {
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (TargetX[k] == Training_Range_Que_X[j] && TargetY[k] == Training_Range_Que_Y[j] ||
                                                PosX[k] == Training_Range_Que_X[j] && PosY[k] == Training_Range_Que_Y[j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        TargetX[i] = Training_Range_Que_X[j];
                                        TargetY[i] = Training_Range_Que_Y[j];
                                        QueingIn[i] = 01;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 2:
                            {
                                for (int j = 0; j < TrainingCriticalQueLength; j++)
                                {
                                    int N = 0;
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (TargetX[k] == Training_Critical_Que_X[N][j] && TargetY[k] == Training_Critical_Que_Y[N][j] ||
                                                PosX[k] == Training_Critical_Que_X[N][j] && PosY[k] == Training_Critical_Que_Y[N][j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        TargetX[i] = Training_Critical_Que_X[N][j];
                                        TargetY[i] = Training_Critical_Que_Y[N][j];
                                        QueingIn[i] = 11;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 3:
                            {
                                for (int j = 0; j < TrainingCriticalQueLength; j++)
                                {
                                    int N = 1;
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (TargetX[k] == Training_Critical_Que_X[N][j] && TargetY[k] == Training_Critical_Que_Y[N][j] ||
                                                PosX[k] == Training_Critical_Que_X[N][j] && PosY[k] == Training_Critical_Que_Y[N][j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        TargetX[i] = Training_Critical_Que_X[N][j];
                                        TargetY[i] = Training_Critical_Que_Y[N][j];
                                        QueingIn[i] = 12;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 4:
                            {
                                for (int j = 0; j < TrainingMeleeQueLength; j++)
                                {
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (TargetX[k] == Training_Melee_Que_X[j] && TargetY[k] == Training_Melee_Que_Y[j] ||
                                                PosX[k] == Training_Melee_Que_X[j] && PosY[k] == Training_Melee_Que_Y[j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        TargetX[i] = Training_Melee_Que_X[j];
                                        TargetY[i] = Training_Melee_Que_Y[j];
                                        QueingIn[i] = 21;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 5:
                            {
                                for (int j = 0; j < TrainingMAQue; j++)
                                {
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (NewPosX[k] == TrainingMA_Que_X[j] && NewPosY[k] == TrainingMA_Que_Y[j] ||
                                                PosX[k] == TrainingMA_Que_X[j] && PosY[k] == TrainingMA_Que_Y[j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        NewPosX[i] = TrainingMA_Que_X[j];
                                        NewPosY[i] = TrainingMA_Que_Y[j];
                                        QueingIn[i] = 31;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 6:
                            {
                                for (int j = 0; j < TrainingAgilityQueLength; j++)
                                {
                                    int N = 0;
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (NewPosX[k] == Training_Agility_Que_X[N][j] && NewPosY[k] == Training_Agility_Que_Y[N][j] ||
                                                PosX[k] == Training_Agility_Que_X[N][j] && PosY[k] == Training_Agility_Que_Y[N][j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        NewPosX[i] = Training_Agility_Que_X[N][j];
                                        NewPosY[i] = Training_Agility_Que_Y[N][j];
                                        QueingIn[i] = 41;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 7:
                            {
                                for (int j = 0; j < TrainingAgilityQueLength; j++)
                                {
                                    int N = 1;
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (NewPosX[k] == Training_Agility_Que_X[N][j] && NewPosY[k] == Training_Agility_Que_Y[N][j] ||
                                                PosX[k] == Training_Agility_Que_X[N][j] && PosY[k] == Training_Agility_Que_Y[N][j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        NewPosX[i] = Training_Agility_Que_X[N][j];
                                        NewPosY[i] = Training_Agility_Que_Y[N][j];
                                        QueingIn[i] = 42;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;

                            case 8:
                            {
                                for (int j = 0; j < TrainingSurvivalQue; j++)
                                {
                                    bool QueOccupied = false;

                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (TargetX[k] == Training_Survival_Que_X[j] && TargetY[k] == Training_Survival_Que_Y[j] ||
                                                PosX[k] == Training_Survival_Que_X[j] && PosY[k] == Training_Survival_Que_Y[j])
                                        {
                                            QueOccupied = true;
                                            break;
                                        }
                                    }

                                    if (!QueOccupied)
                                    {
                                        TargetX[i] = Training_Survival_Que_X[j];
                                        TargetY[i] = Training_Survival_Que_Y[j];
                                        QueingIn[i] = 51;
                                        InQue[i] = true;
                                    }
                                }
                            }
                            break;
                            }
                        }
                    }

                    if (QueingIn[i] > 0)
                    {
                        TrainingText[i] = "Wandering...";
                        if (!WalkedToGoodPlace[i])
                        {
                            if (QueingIn[i] == 01 || QueingIn[i] == 12)
                            {
                                NewPosX[i] = floor(MainDisp/2.5);
                                NewPosY[i] = floor(MainDisp/3);
                            }
                            else if (QueingIn[i] == 21)
                            {
                                NewPosX[i] = floor(MainDisp/2.4);
                                NewPosY[i] = floor(MainDisp-MainDisp/2.7);
                            }
                            else if (QueingIn[i] == 51)
                            {
                                NewPosX[i] = floor(MainDisp-MainDisp/4);
                                NewPosY[i] = floor(MainDisp-MainDisp/2.7);
                            }
                            else if (QueingIn[i] == 11)
                            {
                                NewPosX[i] = floor(MainDisp-MainDisp/4);
                                NewPosY[i] = floor(MainDisp/2.7);
                            }
                        }

                        if (PosX[i] == floor(MainDisp-MainDisp/4)  && PosY[i] == floor(MainDisp/2.7) ||
                                PosX[i] == floor(MainDisp-MainDisp/4)  && PosY[i] == floor(MainDisp-MainDisp/2.7) ||
                                PosX[i] == floor(MainDisp/2.4)         && PosY[i] == floor(MainDisp-MainDisp/2.7) ||
                                PosX[i] == floor(MainDisp/2.5)         && PosY[i] == floor(MainDisp/3))
                        {
                            WalkedToGoodPlace[i] = true;
                            NewPosX[i] = TargetX[i];
                            NewPosY[i] = TargetY[i];
                        }
                    }

                    if (InQue[i])
                    {
                        TrainingText[i] = "Waiting...";
                        switch(QueingIn[i])
                        {
                        case 01:
                        {
                            for (int l = 0; l < TrainingRangeOccupation; l++)
                            {
                                bool TrainingSpaceOccupied = false;
                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if (NewPosX[k] == Training_Range_X[l] && NewPosY[k] == Training_Range_Y[l] ||
                                            PosX[k] == Training_Range_X[l] && PosY[k] == Training_Range_Y[l])
                                    {
                                        TrainingSpaceOccupied = true;
                                        break;
                                    }
                                }

                                for (int j = 0; j < TrainingRangeQueLength; j++)
                                {
                                    if (PosX[i] == Training_Range_Que_X[j] && PosY[i] == Training_Range_Que_Y[j] && !TrainingSpaceOccupied)
                                    {
                                        InQue[i] = false;
                                        QueingIn[i] = 00;
                                        NewPosX[i] = Training_Range_X[l];
                                        NewPosY[i] = Training_Range_Y[l];
                                        InTraining[i] = true;
                                        TrainingIn[i] = 01;
                                    }
                                }
                            }
                        }
                        break;

                        case 11:
                        {
                            int N = 0;
                            for (int l = 0; l < TrainingCriticalOccupation; l++)
                            {
                                bool TrainingSpaceOccupied = false;

                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if (CheckIfInsideCircle(NewPosX[k],NewPosY[k],Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N]) ||
                                            CheckIfInsideCircle(PosX[k],PosY[k],Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N]))
                                    {
                                        TrainingSpaceOccupied = true;
                                        break;
                                    }
                                }

                                for (int j = 0; j < TrainingCriticalQueLength; j++)
                                {
                                    if (PosX[i] == Training_Critical_Que_X[N][j] && PosY[i] ==  Training_Critical_Que_Y[N][j] && !TrainingSpaceOccupied)
                                    {
                                        InQue[i] = false;
                                        QueingIn[i] = 00;
                                        NewPosX[i] = Training_Critical_X[N][N];
                                        NewPosY[i] = Training_Critical_Y[N][N];
                                        InTraining[i] = true;
                                        TrainingIn[i] = 11;
                                    }
                                }
                            }
                        }
                        break;

                        case 12:
                        {
                            int N = 1;
                            for (int l = 0; l < TrainingCriticalOccupation; l++)
                            {
                                bool TrainingSpaceOccupied = false;
                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if (CheckIfInsideCircle(NewPosX[k],NewPosY[k],Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N]) ||
                                            CheckIfInsideCircle(PosX[k],PosY[k],Training_Critical_X[N][N],Training_Critical_Y[N][N],Training_Critical_R[N][N]))
                                    {
                                        TrainingSpaceOccupied = true;
                                        break;
                                    }
                                }

                                for (int j = 0; j < TrainingCriticalQueLength; j++)
                                {
                                    if (PosX[i] == Training_Critical_Que_X[N][j] && PosY[i] ==  Training_Critical_Que_Y[N][j] && !TrainingSpaceOccupied)
                                    {
                                        InQue[i] = false;
                                        QueingIn[i] = 00;
                                        NewPosX[i] = Training_Critical_X[N][N];
                                        NewPosY[i] = Training_Critical_Y[N][N];
                                        InTraining[i] = true;
                                        TrainingIn[i] = 12;
                                    }
                                }
                            }
                        }
                        break;

                        case 21:
                        {
                            int People = 0;
                            for (int k = 0; k < NumberOfPlayers; k++)
                            {
                                if (check(Training_Melee_X1,Training_Melee_Y1,Training_Melee_X1,Training_Melee_Y2,Training_Melee_X2,Training_Melee_Y2,Training_Melee_X2,Training_Melee_Y1,NewPosX[k],NewPosY[k]) ||
                                        check(Training_Melee_X1,Training_Melee_Y1,Training_Melee_X1,Training_Melee_Y2,Training_Melee_X2,Training_Melee_Y2,Training_Melee_X2,Training_Melee_Y1,PosX[k],PosY[k]))
                                {
                                    People++;
                                }
                            }

                            if (People < TrainingMeleeOccupation)
                            {
                                for (int j = 0; j < TrainingMeleeQueLength; j++)
                                {
                                    if (PosX[i] == Training_Melee_Que_X[j] && PosY[i] ==  Training_Melee_Que_Y[j])
                                    {
                                        InQue[i] = false;
                                        QueingIn[i] = 00;
                                        NewPosX[i] = (Training_Melee_X1 + Training_Melee_X2) / 2;
                                        NewPosY[i] = (Training_Melee_Y1 + Training_Melee_Y2) / 2;
                                        InTraining[i] = true;
                                        TrainingIn[i] = 21;
                                    }
                                }
                            }
                        }
                        break;

                        case 31:
                        {
                            bool Occupied = false;

                            for (int k = 0; k < NumberOfPlayers; k++)
                            {
                                if (PosX[k] == TrainingMA_X1 && PosY[k] == TrainingMA_Y1 ||
                                        PosX[k] == TrainingMA_X2 && PosY[k] == TrainingMA_Y2 ||
                                        NewPosX[k] == TrainingMA_X1 && NewPosY[k] == TrainingMA_Y1 ||
                                        NewPosX[k] == TrainingMA_X2 && NewPosY[k] == TrainingMA_Y2)
                                {
                                    Occupied = true;
                                }
                            }

                            for (int j = 0; j < TrainingMAQue; j++)
                            {
                                if (PosX[i] == TrainingMA_Que_X[j] &&
                                        PosY[i] == TrainingMA_Que_Y[j] &&
                                        !Occupied)
                                {
                                    InQue[i] = false;
                                    QueingIn[i] = 00;
                                    NewPosX[i] = TrainingMA_X1;
                                    NewPosY[i] = TrainingMA_Y1;
                                    InTraining[i] = true;
                                    TrainingIn[i] = 31;
                                }
                            }
                        }
                        break;

                        case 41:
                        {
                            int N = 0;
                            bool Occupied = false;

                            for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                            {
                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if (NewPosX[k] == Training_Agility_X[N][j] &&
                                            NewPosY[k] == Training_Agility_Y[N][j] ||
                                            PosX[k] == Training_Agility_X[N][j] &&
                                            PosY[k] == Training_Agility_Y[N][j])
                                    {
                                        Occupied = true;
                                    }
                                }
                            }

                            for (int j = 0; j < TrainingAgilityQueLength; j++)
                            {
                                if (PosX[i] == Training_Agility_Que_X[N][j] &&
                                        PosY[i] == Training_Agility_Que_Y[N][j] &&
                                        !Occupied)
                                {
                                    InQue[i] = false;
                                    QueingIn[i] = 00;
                                    NewPosX[i] = Training_Agility_X[N][0];
                                    NewPosY[i] = Training_Agility_Y[N][0];
                                    InTraining[i] = true;
                                    TrainingIn[i] = 41;
                                }
                            }
                        }
                        break;

                        case 42:
                        {
                            int N = 1;
                            bool Occupied = false;

                            for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                            {
                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if (NewPosX[k] == Training_Agility_X[N][j] &&
                                            NewPosY[k] == Training_Agility_Y[N][j] ||
                                            PosX[k] == Training_Agility_X[N][j] &&
                                            PosY[k] == Training_Agility_Y[N][j])
                                    {
                                        Occupied = true;
                                    }
                                }
                            }

                            for (int j = 0; j < TrainingAgilityQueLength; j++)
                            {
                                if (PosX[i] == Training_Agility_Que_X[N][j] &&
                                        PosY[i] == Training_Agility_Que_Y[N][j] &&
                                        !Occupied)
                                {
                                    InQue[i] = false;
                                    QueingIn[i] = 00;
                                    NewPosX[i] = Training_Agility_X[N][0];
                                    NewPosY[i] = Training_Agility_Y[N][0];
                                    InTraining[i] = true;
                                    TrainingIn[i] = 42;
                                }
                            }
                        }
                        break;

                        case 51:
                        {
                            int People = 0;
                            for (int k = 0; k < NumberOfPlayers; k++)
                            {
                                if (check(Training_Survival_X1,Training_Survival_Y1,Training_Survival_X1,Training_Survival_Y2,Training_Survival_X2,Training_Survival_Y2,Training_Survival_X2,Training_Survival_Y1,NewPosX[k],NewPosY[k]) ||
                                        check(Training_Survival_X1,Training_Survival_Y1,Training_Survival_X1,Training_Survival_Y2,Training_Survival_X2,Training_Survival_Y2,Training_Survival_X2,Training_Survival_Y1,PosX[k],PosY[k]))
                                {
                                    People++;
                                }
                            }

                            if (People < TrainingSurvivalOccupation)
                            {
                                for (int j = 0; j < TrainingSurvivalQue; j++)
                                {
                                    if (PosX[i] == Training_Survival_Que_X[j] && PosY[i] ==  Training_Survival_Que_Y[j])
                                    {
                                        InQue[i] = false;
                                        QueingIn[i] = 00;
                                        NewPosX[i] = (Training_Survival_X1 + Training_Survival_X2) / 2;
                                        NewPosY[i] = (Training_Survival_Y1 + Training_Survival_Y2) / 2;
                                        InTraining[i] = true;
                                        TrainingIn[i] = 51;
                                    }
                                }
                            }
                        }
                        break;
                        }
                    }

                    bool Tr = true;
                    if (InTraining[i])
                    {
                        if (rand()%1000 <= 0 && EnableScream)
                            al_play_sample(TrainingGrunts[rand()%GruntNumber],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                        TrainingText[i] = "Training...";
                        switch (TrainingIn[i])
                        {
                        case 01:
                        {
                            temp_DM[i] += 0.005;

                            Length_Of_Training[i]--;
                            if (Length_Of_Training[i] <= 0)
                                TRDONE = true;
                        }
                        break;

                        case 11:
                        {
                            temp_CC[i] += 0.01;
                            int N = 0;
                            if (PosX[i] == Training_Critical_X[N][N] && PosY[i] == Training_Critical_Y[N][N]
                                    && NewPosX[i] == Training_Critical_X[N][N] && NewPosY[i] == Training_Critical_Y[N][N])
                            {
                                Wander(NewPosX[i],Training_Critical_X[N][N],NewPosY[i],Training_Critical_Y[N][N],Training_Critical_R[N][N],MainDisp);
                            }
                            else if (PosX[i]==NewPosX[i] && PosY[i]==NewPosY[i])
                            {
                                NewPosX[i] = Training_Critical_X[N][N];
                                NewPosY[i] = Training_Critical_Y[N][N];
                            }
                            Length_Of_Training[i]--;
                            if (Length_Of_Training[i] <= 0)
                                TRDONE = true;
                        }
                        break;

                        case 12:
                        {
                            temp_CC[i] += 0.01;
                            int N = 1;
                            if (PosX[i] == Training_Critical_X[N][N] && PosY[i] == Training_Critical_Y[N][N]
                                    && NewPosX[i] == Training_Critical_X[N][N] && NewPosY[i] == Training_Critical_Y[N][N])
                            {
                                Wander(NewPosX[i],Training_Critical_X[N][N],NewPosY[i],Training_Critical_Y[N][N],Training_Critical_R[N][N],MainDisp);
                            }
                            else if (PosX[i]==NewPosX[i] && PosY[i]==NewPosY[i])
                            {
                                NewPosX[i] = Training_Critical_X[N][N];
                                NewPosY[i] = Training_Critical_Y[N][N];
                            }
                            Length_Of_Training[i]--;
                            if (Length_Of_Training[i] <= 0)
                                TRDONE = true;
                        }
                        break;

                        case 21:
                        {
                            temp_DM[i] += 0.01;
                            if (PosX[i] == NewPosX[i] && PosY[i] == NewPosY[i])
                            {
                                WanderSquare(Training_Melee_X1,Training_Melee_Y1,Training_Melee_X2,Training_Melee_Y2,NewPosX[i],NewPosY[i]);
                            }
                            Length_Of_Training[i]--;
                            if (Length_Of_Training[i] <= 0)
                                TRDONE = true;
                        }
                        break;

                        case 31:
                        {


                            if (PosX[i] == TrainingMA_X1 && PosY[i] == TrainingMA_Y1)
                            {
                                NewPosX[i] = TrainingMA_X2;
                                NewPosY[i] = TrainingMA_Y2;
                            }
                            if (PosX[i] == TrainingMA_X2 && PosY[i] == TrainingMA_Y2)
                            {
                                TRDONE = true;
                            }

                            if (NewPosX[i] == TrainingMA_X2 && NewPosY[i] == TrainingMA_Y2)
                            {
                                Tr = false;
                                if (rand()%2 <= 0)
                                {
                                    Walk(NewPosX[i],PosX[i],NewPosY[i],PosY[i]);
                                    temp_DM[i] += 0.005;
                                    temp_DC[i] += 0.005;
                                }
                            }
                        }
                        break;

                        case 41:
                        {
                            temp_DC[i] += 0.01;

                            int N = 0;
                            for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                            {
                                if (PosX[i] == Training_Agility_X[N][TrainingAgilityEdges[N]-1] && PosY[i] == Training_Agility_Y[N][TrainingAgilityEdges[N]-1])
                                {
                                    TRDONE = true;
                                }

                                if (PosX[i] == Training_Agility_X[N][j] && PosY[i] == Training_Agility_Y[N][j])
                                {
                                    NewPosX[i] = Training_Agility_X[N][j+1];
                                    NewPosY[i] = Training_Agility_Y[N][j+1];
                                    break;
                                }
                            }
                        }
                        break;

                        case 42:
                        {
                            temp_DC[i] += 0.01;

                            int N = 1;
                            for (int j = 0; j < TrainingAgilityEdges[N]; j++)
                            {
                                if (PosX[i] == Training_Agility_X[N][TrainingAgilityEdges[N]-1] && PosY[i] == Training_Agility_Y[N][TrainingAgilityEdges[N]-1])
                                {
                                    TRDONE = true;
                                }

                                if (PosX[i] == Training_Agility_X[N][j] && PosY[i] == Training_Agility_Y[N][j])
                                {
                                    NewPosX[i] = Training_Agility_X[N][j+1];
                                    NewPosY[i] = Training_Agility_Y[N][j+1];
                                    break;
                                }
                            }
                        }
                        break;

                        case 51:
                        {
                            temp_SS[i] += 0.01;

                            if (PosX[i] == NewPosX[i] && PosY[i] == NewPosY[i])
                            {
                                WanderSquare(Training_Survival_X1,Training_Survival_Y1,Training_Survival_X2,Training_Survival_Y2,NewPosX[i],NewPosY[i]);
                            }
                            Length_Of_Training[i]--;
                            if (Length_Of_Training[i] <= 0)
                                TRDONE = true;
                        }
                        break;
                        }
                    }

                    if (Day >= 3)
                    {
                        TRDONE = true;
                    }

                    if (TRDONE)
                    {
                        if (TrainingIn[i] > 0 )
                            TrainingText[i] = "Wandering...";

                        if (TrainingIn[i] == 01 || TrainingIn[i] == 12)
                        {
                            NewPosX[i] = floor(MainDisp/3.2);
                            NewPosY[i] = floor(MainDisp/2.7);
                        }
                        else if ((TrainingIn[i] == 31 && Day < 3)|| TrainingIn[i] == 21)
                        {
                            NewPosX[i] = floor(MainDisp/2.4);
                            NewPosY[i] = floor(MainDisp-MainDisp/3);
                        }
                        else if (TrainingIn[i] == 51)
                        {
                            NewPosX[i] = floor(MainDisp-MainDisp/4);
                            NewPosY[i] = floor(MainDisp-MainDisp/2.7);
                        }
                        else if (TrainingIn[i] == 11)
                        {
                            NewPosX[i] = floor(MainDisp-MainDisp/4);
                            NewPosY[i] = floor(MainDisp/2.7);
                        }
                        else
                        {
                            NewPosX[i] = SocialSpaceX[i];
                            NewPosY[i] = SocialSpaceY[i];
                        }
                    }

                    if (TrainingIn[i] > 0 && QueingIn[i] <= 0)
                    {
                        if (PosX[i] == floor(MainDisp-MainDisp/4)  && PosY[i] == floor(MainDisp/2.7) ||
                                PosX[i] == floor(MainDisp-MainDisp/4)  && PosY[i] == floor(MainDisp-MainDisp/2.7) ||
                                PosX[i] == floor(MainDisp/2.4)         && PosY[i] == floor(MainDisp-MainDisp/3) ||
                                PosX[i] == floor(MainDisp/3.2)         && PosY[i] == floor(MainDisp/2.7))
                        {
                            TrainingIn[i] = 0;
                            NewPosX[i] = SocialSpaceX[i];
                            NewPosY[i] = SocialSpaceY[i];
                        }
                    }

                    if (Day >= 3)
                    {
                        TrainingText[i] = "";
                    }

                    if (Day < 3 && Day > 1)
                    {
                        for (int j = 0; j < NumberOfPlayers; j++)
                        {
                            if (PosX[i] == SocialSpaceX[i] && PosY[i] == SocialSpaceY[i] &&
                                    PosX[j] == SocialSpaceX[j] && PosY[j] == SocialSpaceY[j])
                            {
                                if (rand()%2 <= 0)
                                {
                                    temp_RS[i][j]+= 0.01;
                                    temp_RS[j][i]+= 0.01;
                                }
                                else
                                {
                                    temp_RS[i][j]-= 0.01;
                                    temp_RS[j][i]-= 0.01;
                                }
                            }
                        }
                    }

                    if (Tr)
                        Walk(NewPosX[i],PosX[i],NewPosY[i],PosY[i]);

                    al_draw_filled_circle(PosX[i],PosY[i],SizeofChar*1.5,TRIBUTE_COLOR[i]);
                }
            }

            Day += 0.0012;

            int Home = 0;
            for (int i = 0; i < NumberOfPlayers; i++)
            {
                if (PosX[i] == SocialSpaceX[i] && PosY[i] == SocialSpaceY[i])
                    Home++;
                if (Day >= 3 && Home == NumberOfPlayers)
                {
                    al_play_sample(Skills_Reviews,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                    Training = false;
                    Scoring = false;
                    for (int j = 0; j < NumberOfPlayers; j++)
                    {
                        Damage[j]           += temp_DM[j];
                        CritChance[j]       += temp_CC[j];
                        Dodgechance[j]      += temp_DC[j];
                        SurvivalSkill[j]    += temp_SS[j];

                        if (Damage[j] > 10)
                            Damage[j] = 10;
                        if (CritChance[j] > 10)
                            CritChance[j] = 10;
                        if (Dodgechance[j] > 10)
                            Dodgechance[j] = 10;
                        if (SurvivalSkill[j] > 10)
                            SurvivalSkill[j] = 10;

                        for (int k = 0; k < NumberOfPlayers; k++)
                            Relations[j][k]     += temp_RS[j][k];
                    }

                    al_set_target_backbuffer(ThirdDisplay);
                    al_clear_to_color(BLACK);

                    al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,"SCORING TRIBUTES");

                    al_flip_display();
                }
            }
        }

        if (!Scoring)
        {
            al_start_timer(timer);
            Day = 0;
            x = MainDisp/5;
            y = MainDisp/7;

            Printed = false;
            Seconds++;

            for (int i = 0; i < NumberOfPlayers; i+=2)
            {
                if ((Seconds/FPS) == ScoringLength/24*(i+1) && !Printed && Seconds/FPS != Proxy)
                {
                    al_set_target_backbuffer(MainDisplay);
                    al_clear_to_color(BLACK);

                    al_draw_tinted_scaled_bitmap(District_Seal,al_map_rgba(20,20,20,0.5),0,0,500,589,(MainDisp/2)-125,((MainDisp/2)-60)-147.25,250,294.5,NULL);
                    al_draw_text(PREGAME_DISTRICT,al_map_rgba(20,20,20,0.5),MainDisp/2,((MainDisp/2)-60)-FontSize*5.6,ALLEGRO_ALIGN_CENTRE,to_string((i/2)+1).c_str());

                    int score = (11*(Damage[i]+CritChance[i]+Dodgechance[i]+SurvivalSkill[i])/40)+1;
                    al_draw_text(PREGAME_SCORE,WHITE,MainDisp/4,MainDisp/2+120,ALLEGRO_ALIGN_CENTRE,to_string(score).c_str());

                    score = (11*(Damage[i+1]+CritChance[i+1]+Dodgechance[i+1]+SurvivalSkill[i+1])/40)+1;
                    al_draw_text(PREGAME_SCORE,WHITE,MainDisp-MainDisp/4,MainDisp/2+120,ALLEGRO_ALIGN_CENTRE,to_string(score).c_str());

                    al_draw_text(PREGAME_NAME,WHITE,MainDisp/4,MainDisp/2+115,ALLEGRO_ALIGN_CENTRE,FullName[i].c_str());
                    al_draw_text(PREGAME_NAME,WHITE,MainDisp-MainDisp/4,MainDisp/2+115,ALLEGRO_ALIGN_CENTRE,FullName[i+1].c_str());

                    al_draw_tinted_scaled_bitmap(Head[i+1], SKIN_COLOR[i+1],0,0,500,500,(MainDisp-MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);
                    al_draw_tinted_scaled_bitmap(Head[i],   SKIN_COLOR[i],0,0,500,500,(MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);

                    al_draw_tinted_scaled_bitmap(Body[i+1], TRIBUTE_COLOR[i+1],0,0,500,500,(MainDisp-MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);
                    al_draw_tinted_scaled_bitmap(Body[i],   TRIBUTE_COLOR[i],0,0,500,500,(MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);

                    al_draw_tinted_scaled_bitmap(Hair[i+1], HAIR_COLOR[i+1],0,0,500,500,(MainDisp-MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);
                    al_draw_tinted_scaled_bitmap(Hair[i],   HAIR_COLOR[i],0,0,500,500,(MainDisp/4)-100,((MainDisp/2)+50)-100,200,200,NULL);

                    Printed = true;
                    Proxy = Seconds/FPS;
                }
            }
        }

        if (Seconds/FPS >= ScoringLength && !Scoring)
        {
            Seconds = 0;
            Scoring = true;
        }

        if (Scoring && !Training)
        {
            if (!Printed)
            {
                al_set_target_backbuffer(MainDisplay);
                al_clear_to_color(BLACK);

                al_draw_scaled_bitmap(Capitol_Seal,0,0,800,800,MainDisp/2-150,MainDisp/2-150,300,300,NULL);
                al_draw_text(CAPITOL,WHITE,MainDisp/2,MainDisp-MainDisp/5,ALLEGRO_ALIGN_CENTRE,"MAY THE ODDS BE EVER IN YOUR FAVOR");

                al_flip_display();
                Printed = true;
            }

            al_set_target_backbuffer(SecondDisplay);
            al_clear_to_color(BLACK);
            int TextX = FontSize;
            int TextY = 10;

            for (int i = 0; i < NumberOfPlayers; i++)
            {
                al_draw_filled_rectangle(TextX-FontSize/10,TextY,TextX+MaxOnDets+FontSize/10,TextY+FontSize*1.8,al_map_rgb(20,20,20));
                al_draw_text(ROBOTO_REGULAR,TRIBUTE_COLOR[i],TextX,TextY,ALLEGRO_ALIGN_LEFT,FullName[i].c_str());

                int tempS = Seconds;

                if (tempS > 200)
                    tempS = 200;

                al_draw_line(TextX,TextY+FontSize,TextX+tempS/2,TextY+FontSize,al_map_rgb(100,400,300),FontSize/4);
                al_draw_line(TextX,TextY+FontSize*1.4,TextX+tempS/2,TextY+FontSize*1.4,al_map_rgb(0,100,200),FontSize/4);
                al_draw_line(TextX,TextY+FontSize*1.6,TextX+tempS/2,TextY+FontSize*1.6,al_map_rgb(500,200,300),FontSize/4);

                TextY = TextY + FontSize/2;
                TextY = TextY - FontSize/2.5;
                TextX = TextX + SecDispV/2;

                if (i%2 != 0)
                {
                    TextX = FontSize;
                    TextY = TextY + FontSize + FontSize/1.1;
                }
            }
            al_flip_display();

            al_set_target_backbuffer(ThirdDisplay);
            al_clear_to_color(BLACK);

            al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,"Preparing Arena");

            al_flip_display();

            int limit = 10*FPS;
            Seconds++;

            if (Seconds >= limit)
            {
                Seconds = 0;
                PreGame = false;
                Printed = false;
                break;
            }
        }

        al_flip_display();

        if (Training)
        {
            al_set_target_backbuffer(SecondDisplay);
            al_clear_to_color(BLACK);

            int TextX = FontSize;
            int TextY = 10;

            for (int i = 0; i < NumberOfPlayers; i++)
            {
                al_draw_filled_rectangle(TextX-FontSize/10,TextY,TextX+MaxOnDets+FontSize/10,TextY+FontSize*1.8,al_map_rgb(20,20,20));

                al_draw_text(ROBOTO_REGULAR,TRIBUTE_COLOR[i],TextX,TextY,ALLEGRO_ALIGN_LEFT,FullName[i].c_str());
                al_draw_text(ROBOTO_REGULAR,TRIBUTE_COLOR[i],TextX,TextY+FontSize,ALLEGRO_ALIGN_LEFT,TrainingText[i].c_str());

                TextY = TextY + FontSize/2;
                TextY = TextY - FontSize/2.5;
                TextX = TextX + SecDispV/2;

                if (i%2 != 0)
                {
                    TextX = FontSize;
                    TextY = TextY + FontSize + FontSize/1.1;
                }
            }
            al_flip_display();

            al_set_target_backbuffer(ThirdDisplay);
            al_clear_to_color(BLACK);

            string Word = "TRAINING DAY: " + to_string(int(Day));
            al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/5,ALLEGRO_ALIGN_CENTRE,Word.c_str());
            al_draw_line(SecDispV/10,ThiDispH/5+FontSize*1.5,SecDispV-(SecDispV/10),ThiDispH/5+FontSize*1.5,WHITE,5);

            int x = SecDispV/34;
            int y;
            int a;

            for (int i = 0; i < 10; i++)
            {
                y = ThiDispH/5+FontSize*2;
                x += 21;
                for (int i = 0; i < 2; i++)
                {
                    if(rand()%50<=0)
                        a = 30;
                    else
                        a = 60;
                    al_draw_filled_rectangle(x,y,x+20,y+20,al_map_rgb(a,a,a));
                    y += 21;
                }
                if(rand()%50<=0)
                    a = 30;
                else
                    a = 60;
                al_draw_filled_rectangle(x,y,x+20,y+20,al_map_rgb(a,a,a));
            }

            al_flip_display();
        }

        al_register_event_source(Que, al_get_display_event_source(MainDisplay));
        al_register_event_source(Que, al_get_display_event_source(SecondDisplay));
        al_register_event_source(Que, al_get_display_event_source(ThirdDisplay));
        //To Close
        al_wait_for_event(Que, &Event);
        if(Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            PreGame = false;
            running = false;
        }
    }

    al_stop_timer(timer);
    al_start_timer(timer);

    //GameSimulation = false;
    //TheFallenDone = true;
    //SongLength = 1;

    /*for (int i = 0; i < NumberOfPlayers; i++)
        {
            DeathOrder[i] = i; DeathText[i] = "I was killed huhuhu";
        }*/

    ///SET START POSITION OF ALL CHARACTER/TRIBUTES
    for (int i = 0; i < NumberOfPlayers; i++)
    {
        SetStartPosition(PosX[i],PosY[i],NumberOfPlayers,MainDisp,MainDisp/5.5,i,CornucopiaAngle,Angle[i]);
        switch (Organization[i])
        {
        case 'P':
            SetStartMovement(NewPosX[i],NewPosY[i],NumberOfPlayers,MainDisp,MainDisp/50,i,CornucopiaAngle,Angle[i]);
            //SetStartPosition(NewPosX[i],NewPosY[i],NumberOfPlayers,MainDisp,MainDisp/5.5,i,CornucopiaAngle,Angle[i]);
            break;
        case 'J':
            SetStartMovement(NewPosX[i],NewPosY[i],NumberOfPlayers,MainDisp,MainDisp/5.5,i,CornucopiaAngle,Angle[i]);
            break;
        }
    }

    ///THE GAME -> END GAME
    while(running)
    {
        //Main Screen
        al_set_target_backbuffer(MainDisplay);

        if (GameSimulation && CountDown)
        {
            if (CountDown)
            {
                CountDown = false;
            }

            if (AlivePlayers <= 1)
            {
                GameSimulation = false;
            }

            if (!PlayedAmbient)
            {
                al_play_sample(Ambient,1,0,1,ALLEGRO_PLAYMODE_LOOP,NULL);
                PlayedAmbient = true;
            }

            if (rand()%100000 <= 0)
                al_play_sample(Mockingjay,0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

            al_clear_to_color(ARENA);

            AlivePlayers = 0;
            for (int i = 0; i < NumberOfPlayers; i++)
                if(Status[i] == 1)
                    AlivePlayers++;

            PrintCornucopia(CornucopiaX,CornucopiaY,CornucopiaX2,CornucopiaY2,CornucopiaX3,CornucopiaY3,CornucopiaR,ARENA,CornucopiaAngle);
            //al_draw_filled_circle(MainDisp/2,MainDisp/2,MainDisp/20,BLACK);
            al_draw_circle(MainDisp/2,MainDisp/2,MainDisp/2,WHITE,1);       //Arena Size
            //al_draw_circle(MainDisp/2,MainDisp/2,MainDisp/4,WHITE,1);       //Cornucopia Size
            //al_draw_circle(MainDisp/2,MainDisp/2,MainDisp/5,WHITE,1);     //Player Position Circle //Remove once done

            for (int i = 0; i < NUMBER_OF_PONDS; i++)
                for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
                {
                    PrintEnvironmentPond(PondX[i],PondY[i],PondR[i]);
                    PrintEnvironmentPond(ExtraPondX[i][j],ExtraPondY[i][j],ExtraPondR[i][j]);
                }

            //     if (Day > 0.5)
            //         PadestalR -= 0.01;

            for (int i = 0; i < NumberOfPlayers; i++)
                al_draw_filled_circle(PadestalX[i],PadestalY[i],PadestalR,Padestal);

            for (int i = 0; i < NumberOfPlayers; i++)
            {
                HasMWeapon[i] =
                    EquipedWeapon[i].Knife+
                    EquipedWeapon[i].Mace+
                    EquipedWeapon[i].Machete+
                    EquipedWeapon[i].Sickle+
                    EquipedWeapon[i].Sword+

                    EquipedWeapon[i].Axe+
                    EquipedWeapon[i].Dagger+
                    EquipedWeapon[i].Trident+
                    EquipedWeapon[i].Spear;

                HasRWeapon[i] =
                    EquipedWeapon[i].Bow+
                    EquipedWeapon[i].ThrKnife+

                    EquipedWeapon[i].ThrAxe+
                    EquipedWeapon[i].ThrSpear+
                    EquipedWeapon[i].ThrTrident;
                switch(Status[i])
                {
                case 0:
                {
                    Health[i] = 0;

                    al_draw_filled_circle(PosX[i],PosY[i],SizeofChar,GREY);

                    if (DeathOrder[i] == 0)
                    {
                        DeathOrder[i] = DeathNumber;
                        DeathNumber++;
                    }

                    ///Food Drop
                    if (Inventory[i].Food > 1)
                    {
                        for (int j = 0; j < NumberOfFood; j++)
                        {
                            if (Inventory[i].Food <= 0)
                                break;
                            if (FoodPosX[j] == -999 && FoodPosY[j] == -999)
                            {
                                FoodPosX[j] = PosX[i]+((rand()%20)-10);
                                FoodPosY[j] = PosY[i]+((rand()%20)-10);
                                Inventory[i].Food--;
                            }
                        }
                    }

                    ///Night lock drop
                    if (Inventory[i].Nightlock > 1)
                    {
                        for (int j = 0; j < NumberOfNightlocks; j++)
                        {
                            if (Inventory[i].Nightlock <= 0)
                                break;
                            if (NightlockPosX[j] == -999 && NightlockPosY[j] == -999)
                            {
                                NightlockPosX[j] = PosX[i]+((rand()%20)-10);
                                NightlockPosY[j] = PosY[i]+((rand()%20)-10);
                                Inventory[i].Nightlock--;
                            }
                        }
                    }

                    ///Med pack drop
                    if (Inventory[i].Medpack > 1)
                    {
                        for (int j = 0; j < NumberOfMedpacks; j++)
                        {
                            if (Inventory[i].Medpack <= 0)
                                break;
                            if (MedpackPosX[j] == -999 && MedpackPosY[j] == -999)
                            {
                                MedpackPosX[j] = PosX[i]+((rand()%20)-10);
                                MedpackPosY[j] = PosY[i]+((rand()%20)-10);
                                Inventory[i].Medpack--;
                            }
                        }
                    }

                    ///Backpack drop
                    if (Inventory[i].Backpack)
                    {
                        for (int j = 0; j < NumberOfBackpacks; j++)
                        {
                            if (!Inventory[i].Backpack)
                                break;
                            if (BackpackX[j] == -999 && BackpackY[j] == -999)
                            {
                                BackpackX[j] = PosX[i]+((rand()%20)-10);
                                BackpackY[j] = PosY[i]+((rand()%20)-10);
                                Inventory[i].Backpack = false;
                            }
                        }
                    }

                    ///Melee Weapon Drop
                    if (HasMWeapon[i] >= 1)
                    {
                        for (int j = 0; j < NumberOfWeapons; j++)
                        {
                            if (WeaponX[j] == -999 + i && WeaponY[j] == -999 + i)
                            {
                                WeaponX[j] = PosX[i]+((rand()%20)-10);
                                WeaponY[j] = PosY[i]+((rand()%20)-10);
                                EquipedWeapon[i].Knife = false;
                                EquipedWeapon[i].Mace = false;
                                EquipedWeapon[i].Machete = false;
                                EquipedWeapon[i].Sickle = false;
                                EquipedWeapon[i].Sword = false;

                                EquipedWeapon[i].Axe = false;
                                EquipedWeapon[i].Dagger = false;
                                EquipedWeapon[i].Trident = false;
                                EquipedWeapon[i].Spear = false;
                            }
                        }
                    }

                    ///Range Weapon Drop
                    if (HasRWeapon[i] >= 1)
                    {
                        for (int j = 0; j < NumberOfWeapons; j++)
                        {
                            if (WeaponX[j] == -999 + i && WeaponY[j] == -999 + i)
                            {
                                WeaponX[j] = PosX[i]+((rand()%20)-10);
                                WeaponY[j] = PosY[i]+((rand()%20)-10);
                                EquipedWeapon[i].Bow = false;
                                EquipedWeapon[i].ThrKnife = false;

                                EquipedWeapon[i].ThrSpear = false;
                                EquipedWeapon[i].ThrTrident = false;
                                EquipedWeapon[i].ThrAxe = false;
                            }
                        }
                    }
                }

                {
                    int SeeDeadBody = 0;
                    for (int j = 0; j < NumberOfPlayers; j++)
                        if ((pow((PosX[j] - PosX[i]),2) + pow((PosY[j] - PosY[i]),2) <= pow(AREA_RADIUS,2)) && i != j && Status[j] == 1)
                            SeeDeadBody++;

                    if (SeeDeadBody <= 0)
                    {
                        PosX[i] = -999;
                        PosY[i] = -999;
                    }
                }

                break;

                case 1:
                    //Print Character
                    al_draw_filled_circle(PosX[i],PosY[i],SizeofChar,TRIBUTE_COLOR[i]);
                    //al_draw_text(DAYCOUNTER,WHITE,PosX[i],PosY[i], ALLEGRO_ALIGN_CENTER,FirstName[i].c_str());
                    //al_draw_filled_circle(PosX[i],PosY[i],SizeofChar/2,WHITE);
                    //Print Area of possible next position via Wonder()
                    //al_draw_circle(PosX[i],PosY[i],AREA_RADIUS,WHITE,1);
                    //Draw line from character to next position
                    //al_draw_line(PosX[i],PosY[i],NewPosX[i],NewPosY[i],WHITE,1);

                    if (NewPosY[i] >= PosY[i])
                        NewAngle[i] = abs(atan2(NewPosY[i]-PosY[i],NewPosX[i]-PosX[i])) - ANGLE_OFFSET/2;

                    else if (NewPosY[i] < PosY[i])
                        NewAngle[i] = abs(atan2(PosY[i]-NewPosY[i],PosX[i]-NewPosX[i])) + ANGLE_OFFSET + ANGLE_OFFSET/2;

                    if (Angle[i] != NewAngle[i] && NewAngle[i] != -(ANGLE_OFFSET - ANGLE_OFFSET/2))
                        Angle[i] = NewAngle[i];

                    ///First Angle (Vision)
                    AngleX[i] = PosX[i] + AREA_RADIUS*(cos(Angle[i]));
                    AngleY[i] = PosY[i] + AREA_RADIUS*(sin(Angle[i]));
                    ///Second Angle (Vision)
                    AngleX2[i] = PosX[i] + AREA_RADIUS*(cos(Angle[i]+ANGLE_OFFSET));
                    AngleY2[i] = PosY[i] + AREA_RADIUS*(sin(Angle[i]+ANGLE_OFFSET));

                    ///Edge of Square
                    AngleXX[i] = AngleX[i] + AREA_RADIUS*(cos(Angle[i]+1.56));
                    AngleYY[i] = AngleY[i] + AREA_RADIUS*(sin(Angle[i]+1.56));

                    //Printing the FOV ///Remove when unnecessary
                    /*al_draw_line(PosX[i],PosY[i],AngleX[i],AngleY[i],WHITE,1);
                    al_draw_line(PosX[i],PosY[i],AngleX2[i],AngleY2[i],WHITE,1);
                    al_draw_line(AngleX[i],AngleY[i],AngleXX[i],AngleYY[i],WHITE,1);
                    al_draw_line(AngleX2[i],AngleY2[i],AngleXX[i],AngleYY[i],WHITE,1);
                    */

                    ZeroValX[i] = PosX[i] - AREA_RADIUS;
                    ZeroValY[i] = PosY[i] - AREA_RADIUS;

                    MaxValX[i] = PosX[i] + AREA_RADIUS;
                    MaxValY[i] = PosY[i] + AREA_RADIUS;

                    ///Check if Not Valid
                    if ((PosX[i] >= MaxValX[i]) && (PosX[i] < ZeroValX[i]) && (PosY[i] >= MaxValY[i]) && (PosY[i] < ZeroValY[i]))
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                    ///Check if Not Valid
                    if ((NewPosX[i] >= MaxValX[i]) && (NewPosX[i] < ZeroValX[i]) && (NewPosY[i] >= MaxValY[i]) && (NewPosY[i] < ZeroValY[i]))
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                    ///Check if Blocked
                    if (Grid[PosX[i]][PosY[i]] == 1 || Grid[NewPosX[i]][NewPosY[i]] == 1)
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                    ///Check if Character is on Destination
                    if (PosX[i] == NewPosX[i] && PosY[i] == NewPosY[i])
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);

                    /*
                    ///Check if Character is inside the Cornucopia
                    if (isInside(CornucopiaX,CornucopiaY,CornucopiaX2,CornucopiaY2,CornucopiaX3,CornucopiaY3,PosX[i],PosY[i]))
                    Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);

                    ///Check if Next Position is inside the Cornucopia
                    if (isInside(CornucopiaX,CornucopiaY,CornucopiaX2,CornucopiaY2,CornucopiaX3,CornucopiaY3,NewPosX[i],NewPosY[i]))
                    Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                    */

                    ///NIGHTLOCK INTERACTION
                    for (int j = 0; j < NumberOfNightlocks; j++)
                    {
                        if (pow((PosX[i] - NightlockPosX[j]),2) + pow((PosY[i] - NightlockPosY[j]),2) <= pow(AREA_RADIUS,2) &&
                                check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], NightlockPosX[j], NightlockPosY[j]))
                        {
                            if (Inventory[i].Nightlock < 1 || (Inventory[i].Nightlock < 2 && Inventory[i].Backpack))
                            {
                                NewPosX[i] = NightlockPosX[j];
                                NewPosY[i] = NightlockPosY[j];

                                if (Grid[PosX[i]][PosY[i]-1] == 1 || Grid[PosX[i]+1][PosY[i]] == 1 || Grid[PosX[i]-1][PosY[i]] == 1 || Grid[PosX[i]+1][PosY[i]] == 1)
                                {
                                    Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                                }

                                if (PosX[i] == NightlockPosX[j] && PosY[i] == NightlockPosY[j] && Inventory[i].Nightlock < 1)
                                {
                                    NightlockPosX[j] = -999;
                                    NightlockPosY[j] = -999;
                                    Inventory[i].Nightlock++;
                                }
                                else if (PosX[i] == NightlockPosX[j] && PosY[i] == NightlockPosY[j] && Inventory[i].Nightlock < 3 && Inventory[i].Backpack)
                                {
                                    NightlockPosX[j] = -999;
                                    NightlockPosY[j] = -999;
                                    Inventory[i].Nightlock++;
                                }
                            }
                        }
                    }

                    ///FOOD INTERACTION
                    for (int j = 0; j < NumberOfFood; j++)
                    {
                        if (pow((PosX[i] - FoodPosX[j]),2) + pow((PosY[i] - FoodPosY[j]),2) <= pow(AREA_RADIUS,2) &&
                                check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], FoodPosX[j], FoodPosY[j]))
                        {
                            if (Inventory[i].Food < 1 || (Inventory[i].Food < 2 && Inventory[i].Backpack))
                            {
                                NewPosX[i] = FoodPosX[j];
                                NewPosY[i] = FoodPosY[j];

                                if (Grid[PosX[i]][PosY[i]-1] == 1 || Grid[PosX[i]+1][PosY[i]] == 1 || Grid[PosX[i]-1][PosY[i]] == 1 || Grid[PosX[i]+1][PosY[i]] == 1)
                                {
                                    Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                                }

                                if (PosX[i] == FoodPosX[j] && PosY[i] == FoodPosY[j] && Inventory[i].Food < 1)
                                {
                                    FoodPosX[j] = -999;
                                    FoodPosY[j] = -999;
                                    Inventory[i].Food++;
                                }
                                else if (PosX[i] == FoodPosX[j] && PosY[i] == FoodPosY[j] && Inventory[i].Food < 3 && Inventory[i].Backpack)
                                {
                                    FoodPosX[j] = -999;
                                    FoodPosY[j] = -999;
                                    Inventory[i].Food++;
                                }
                            }
                        }
                    }

                    ///WATER INTERACTION
                    for (int j = 0; j < NUMBER_OF_PONDS; j++)
                    {
                        for (int k = 0; k < NUMBER_OF_PONDS_INSIDE_PONDS; k++)
                        {
                            if (Inventory[i].Water <= 0 && abs(sqrt(pow(PosX[i]-ExtraPondX[j][k],2)+ pow(ExtraPondY[j][k]-PosY[i],2))) <= AREA_RADIUS + ExtraPondR[j][k]
                                    || abs(sqrt(pow(PosX[i]-PondX[j],2)+ pow(PondY[j]-PosY[i],2))) <= AREA_RADIUS + PondR[j])
                            {
                                if (WaterNeed[i] < 50 && Inventory[i].Water <= 0)
                                {
                                    NewPosX[i] = PondX[j];
                                    NewPosY[i] = PondY[j];
                                }

                                if (abs(sqrt(pow(PosX[i]-ExtraPondX[j][k],2)+ pow(ExtraPondY[j][k]-PosY[i],2))) <= ExtraPondR[j][k]
                                        || abs(sqrt(pow(PosX[i]-PondX[j],2)+ pow(PondY[j]-PosY[i],2))) <= PondR[j])
                                {
                                    if (WaterNeed[i] < MaxOnDets && Inventory[i].Water < 1)// || (WaterNeed[i] < 101 && Inventory[i].Water < 1))
                                    {
                                        NewPosX[i] = PosX[i];
                                        NewPosY[i] = PosY[i];
                                        WaterNeed[i]++;
                                    }

                                    if (WaterNeed[i] >= MaxOnDets && Inventory[i].Water < 1)
                                        Inventory[i].Water++;

                                    if (WaterNeed[i] >= MaxOnDets && Inventory[i].Water < 3 && Inventory[i].Backpack)
                                        Inventory[i].Water++;

                                    //else
                                    //Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);

                                    Memory[i].WaterX = PosX[i];
                                    Memory[i].WaterY = PosY[i];

                                }
                            }
                            else if ((Memory[i].WaterX != -1 && Memory[i].WaterY != -1) && (WaterNeed[i] < 50 && Inventory[i].Water < 1))
                            {
                                NewPosX[i] = Memory[i].WaterX;
                                NewPosY[i] = Memory[i].WaterY;
                            }
                        }
                    }

                    ///MEDPACK INTERACTION
                    for (int j = 0; j < NumberOfMedpacks; j++)
                    {
                        if (pow((PosX[i] - MedpackPosX[j]),2) + pow((PosY[i] - MedpackPosY[j]),2) <= pow(AREA_RADIUS,2) &&
                                check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], MedpackPosX[j], MedpackPosY[j]))
                        {
                            if (Inventory[i].Medpack < 1 || (Inventory[i].Medpack < 2 && Inventory[i].Backpack))
                            {
                                NewPosX[i] = MedpackPosX[j];
                                NewPosY[i] = MedpackPosY[j];

                                if (PosX[i] == MedpackPosX[j] && PosY[i] == MedpackPosY[j] && Inventory[i].Medpack < 1)
                                {
                                    MedpackPosX[j] = -999;
                                    MedpackPosY[j] = -999;
                                    Inventory[i].Medpack++;
                                }
                                else if (PosX[i] == MedpackPosX[j] && PosY[i] == MedpackPosY[j] && Inventory[i].Medpack < 2 && Inventory[i].Backpack)
                                {
                                    MedpackPosX[j] = -999;
                                    MedpackPosY[j] = -999;
                                    Inventory[i].Medpack++;
                                }
                            }
                        }
                    }

                    ///BACKPACK INTERACTION
                    for (int j = 0; j < NumberOfBackpacks; j++)
                    {
                        if (pow((PosX[i] - BackpackX[j]),2) + pow((PosY[i] - BackpackY[j]),2) <= pow(AREA_RADIUS,2) &&
                                check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], BackpackX[j], BackpackY[j])
                                && !Inventory[i].Backpack)
                        {
                            NewPosX[i] = BackpackX[j];
                            NewPosY[i] = BackpackY[j];

                            if (PosX[i] == BackpackX[j] && PosY[i] == BackpackY[j])
                            {
                                BackpackX[j] = -999;
                                BackpackY[j] = -999;
                                Inventory[i].Backpack = true;
                                Inventory[i].Food++;
                                Inventory[i].Water++;
                            }
                        }
                    }

                    ///OTHER PLAYER/TRIBUTE INTERACTION
                    int Target = 0;

                    for (int j = 0; j < NumberOfPlayers; j++)
                    {
                        Relations[i][j] -= RelationsDecrementation[i];

                        if (Relations[i][j] <= 0 )//|| (Decision[i] == 'T' && Day < 1))
                        {
                            if ((pow((PosX[i] - PosX[j]),2) + pow((PosY[i] - PosY[j]),2) <= pow(AREA_RADIUS,2) &&
                                    check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], PosX[j], PosY[j]) &&
                                    i != j && Status[j] == 1) ||
                                    (pow((PosX[i] - PosX[j]),2) + pow((PosY[i] - PosY[j]),2) <= pow(AREA_RADIUS/4,2) && Status[j] == 1 && i != j))
                            {
                                Target = j;
                                for (int k = 0; k < NumberOfPlayers; k++)
                                {
                                    if ((pow((PosX[i] - PosX[k]),2) + pow((PosY[i] - PosY[k]),2) <= pow(AREA_RADIUS,2) &&
                                            check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], PosX[k], PosY[k]) &&
                                            i != k && Status[k] == 1) ||
                                            (pow((PosX[i] - PosX[k]),2) + pow((PosY[i] - PosY[k]),2) <= pow(AREA_RADIUS/5,2) && Status[k] == 1 && i != k))
                                    {
                                        if (sqrt((pow(PosX[i]-PosX[Target],2)+pow(PosY[i]-PosY[Target],2)))
                                                >= sqrt((pow(PosX[i]-PosX[k],2)+pow(PosY[i]-PosY[k],2))))
                                            Target = k;
                                    }
                                }

                                Relations[i][Target] = 0;
                                Relations[Target][i] = 0;

                                ///Check if in range. and if it is in range
                                if (HasRWeapon[i] >= 1 && Status[Target] &&
                                        pow((PosX[i] - PosY[Target]),2) + pow((PosY[i] - PosY[Target]),2) <= pow(AREA_RADIUS/2,2)
                                        && pow((PosX[i] - PosY[Target]),2) + pow((PosY[i] - PosY[Target]),2) >= pow(AREA_RADIUS/3,2))
                                {

                                    //NewPosX[i] = PosX[i];
                                    //NewPosY[i] = PosY[i];

                                    FoodNeed[i] -= WaterDeteriorationRate[i]*2;
                                    WaterNeed[i] -= WaterDeteriorationRate[i]*2;

                                    if (Attack[i] >= AttackDelay[i])
                                    {
                                        Relations[Target][i] = 0;
                                        Attack[i] = 0;

                                        if (Dodgechance[Target]*2 > rand()%100)
                                        {
                                            Health[Target] += HealthRegeneration[Target];
                                            Health[i] += HealthRegeneration[i];
                                        }

                                        else if (CritChance[i] > rand()%100)
                                        {
                                            Health[Target] -= Damage[i]*2;
                                        }

                                        else
                                            Health[Target] -= Damage[i];
                                    }
                                    else
                                        Attack[i]++;

                                    if (Health[Target] <= 0)
                                    {
                                        if (EnableScream)
                                            al_play_sample(SoundDeath[GenderType[Target]][rand()%DeathNum[GenderType[Target]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                                        DeathText[Target] = FirstName[Target] + " was killed by " + FirstName[i] + " using ";
//                                   cout << FirstName[Target] << " was killed by " << FirstName[i] << " using ";
                                        if (EquipedWeapon[i].Bow)
                                            DeathText[Target] += "a Bow";
                                        else if (EquipedWeapon[i].ThrKnife)
                                            DeathText[Target] += "a Throwing Knife";

                                        else if (EquipedWeapon[i].ThrAxe)
                                            DeathText[Target] += "a Throwing Axe";
                                        else if (EquipedWeapon[i].ThrSpear)
                                            DeathText[Target] += "a Throwing Spear";
                                        else if (EquipedWeapon[i].ThrTrident)
                                            DeathText[Target] += "a Trident";

                                        //cout << "\n";
                                        KillCount[i]++;
                                        Status[Target] = 0;
                                        Health[Target] = 0;

                                    }
                                }

                                ///Check if melee
                                else if (HasMWeapon[i] >= 1 || Day > 2)
                                {
                                    if (Day >= 1)
                                    {
                                        if (Information[i] == 'S' && Health[i] >= Health[Target])
                                        {
                                            NewPosX[i] = PosX[Target];
                                            NewPosY[i] = PosY[Target];
                                        }
                                        else if (Information[i] == 'N' && WaterNeed[i] >= MaxOnDets/3 && FoodNeed[i] >= MaxOnDets/3)
                                        {
                                            NewPosX[i] = PosX[Target];
                                            NewPosY[i] = PosY[Target];
                                        }
                                    }
                                    else
                                    {
                                        NewPosX[i] = PosX[Target];
                                        NewPosY[i] = PosY[Target];
                                    }
                                }

                                if (PosX[i] == PosX[Target] && PosY[i] == PosY[Target] && Status[Target] == 1 && (HasMWeapon [i] >= 1 || Day > 2))
                                {
                                    NewPosX[i] = PosX[i];
                                    NewPosY[i] = PosY[i];

                                    PosX[Target] = PosX[Target];
                                    PosY[Target] = PosY[Target];

                                    FoodNeed[i] -= WaterDeteriorationRate[i]*2;
                                    WaterNeed[i] -= WaterDeteriorationRate[i]*2;

                                    //Damage
                                    if (Attack[i] >= AttackDelay[i])
                                    {
                                        Relations[Target][i] = 0;
                                        Attack[i] = 0;

                                        if (Dodgechance[Target] > rand()%100)
                                        {
                                            Health[Target] += HealthRegeneration[Target];
                                            Health[i] += HealthRegeneration[i];
                                        }

                                        else if (CritChance[i] > rand()%100)
                                        {
                                            Health[Target] -= Damage[i]*2;
                                        }

                                        else
                                            Health[Target] -= Damage[i];
                                    }
                                    else
                                        Attack[i]++;

                                    if (Health[Target] <= 0)
                                    {
                                        if (EnableScream)//&& rand()%5 <= 0 || (Day > 2 && EnableScream))
                                            al_play_sample(SoundDeath[GenderType[Target]][rand()%DeathNum[GenderType[Target]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                                        //cout << FirstName[Target] << " was killed by " << FirstName[i] << " using ";
                                        DeathText[Target] = FirstName[Target] + " was killed by " + FirstName[i] + " using ";
                                        if (EquipedWeapon[i].Knife)
                                            DeathText[Target] += "a Knife";
                                        else if (EquipedWeapon[i].Machete)
                                            DeathText[Target] += "a Machete";
                                        else if (EquipedWeapon[i].Sickle)
                                            DeathText[Target] += "a Sickle";
                                        else if (EquipedWeapon[i].Sword)
                                            DeathText[Target] += "a Sword";
                                        else if (EquipedWeapon[i].Mace)
                                            DeathText[Target] += "a Mace";
                                        else if (EquipedWeapon[i].Axe)
                                            DeathText[Target] += "an Axe";
                                        else if (EquipedWeapon[i].Dagger)
                                            DeathText[Target] += "a Dagger";
                                        else if (EquipedWeapon[i].Spear)
                                            DeathText[Target] += "a Spear";
                                        else if (EquipedWeapon[i].Trident)
                                            DeathText[Target] += "a Trident";

                                        else if (!EquipedWeapon[i].Mace && !EquipedWeapon[i].Sword &&
                                                 !EquipedWeapon[i].Sickle && !EquipedWeapon[i].Machete &&
                                                 !EquipedWeapon[i].Knife && !EquipedWeapon[i].Axe &&
                                                 !EquipedWeapon[i].Dagger&& !EquipedWeapon[i].Spear&&
                                                 !EquipedWeapon[i].Trident)
                                        {
                                            int a = rand()%3;
                                            switch (a)
                                            {
                                            case 0:
                                                DeathText[Target] += "their fists";
                                                break;
                                            case 1:
                                                DeathText[Target] += "a Rock";
                                                break;
                                            case 2:
                                                DeathText[Target] += "a Stick";
                                                break;
                                            }
                                        }

                                        if (Target == i)
                                            DeathText[Target] = FirstName[Target] + " died from fatigue";

                                        else
                                            KillCount[i]++;

                                        Status[Target] = 0;
                                        Health[Target] = 0;

                                    }
                                }
                            }
                        }
                    }

                    ///INTERACTION WITH WEAPONS
                    for (int j = 0; j < NumberOfWeapons; j++)
                    {
                        if (pow((PosX[i] - WeaponX[j]),2) + pow((PosY[i] - WeaponY[j]),2) <= pow(AREA_RADIUS,2) &&
                                check(PosX[i], PosY[i], AngleX[i], AngleY[i], AngleX2[i], AngleY2[i], AngleXX[i], AngleYY[i], WeaponX[j], WeaponY[j]))
                        {
                            if (HasMWeapon[i] <= 0 && j < NumberOfMelee)
                            {
                                NewPosX[i] = WeaponX[j];
                                NewPosY[i] = WeaponY[j];
                                if (WeaponX[j] == PosX[i] && WeaponY[j] == PosY[i])
                                {
                                    WeaponX[j] = -999 + i;
                                    WeaponY[j] = -999 + i;
                                    if (WeaponInfo[j] == 1)
                                    {
                                        switch (WeaponType[j])
                                        {
                                        case 1:
                                            EquipedWeapon[i].Knife = true;
                                            Damage[i] += MeleeWeapon.Knife;
                                            AttackDelay[i] = MeleeWeapon.Knife;
                                            break;
                                        case 2:
                                            EquipedWeapon[i].Machete = true;
                                            Damage[i] += MeleeWeapon.Machete;
                                            AttackDelay[i] = MeleeWeapon.Machete;
                                            break;
                                        case 3:
                                            EquipedWeapon[i].Sword = true;
                                            Damage[i] += MeleeWeapon.Sword;
                                            AttackDelay[i] = MeleeWeapon.Sword;
                                            break;
                                        case 4:
                                            EquipedWeapon[i].Sickle = true;
                                            Damage[i] += MeleeWeapon.Sickle;
                                            AttackDelay[i] = MeleeWeapon.Sickle;
                                            break;
                                        case 5:
                                            EquipedWeapon[i].Mace = true;
                                            Damage[i] += MeleeWeapon.Mace;
                                            AttackDelay[i] = MeleeWeapon.Mace;
                                            break;
                                        case 6:
                                            EquipedWeapon[i].Axe = true;
                                            Damage[i] += MeleeWeapon.Axe;
                                            AttackDelay[i] = MeleeWeapon.Axe;
                                            break;
                                        case 7:
                                            EquipedWeapon[i].Dagger = true;
                                            Damage[i] += MeleeWeapon.Dagger;
                                            AttackDelay[i] = MeleeWeapon.Dagger;
                                            break;
                                        case 8:
                                            EquipedWeapon[i].Trident = true;
                                            Damage[i] += MeleeWeapon.Trident;
                                            AttackDelay[i] = MeleeWeapon.Trident;
                                            break;
                                        case 9:
                                            EquipedWeapon[i].Spear = true;
                                            Damage[i] += MeleeWeapon.Spear;
                                            AttackDelay[i] = MeleeWeapon.Spear;
                                            break;
                                        }
                                    }
                                }
                            }
                            if (HasRWeapon[i] <= 0 && j >= NumberOfMelee)
                            {
                                NewPosX[i] = WeaponX[j];
                                NewPosY[i] = WeaponY[j];
                                if (WeaponX[j] == PosX[i] && WeaponY[j] == PosY[i])
                                {
                                    WeaponX[j] = -999 + i;
                                    WeaponY[j] = -999 + i;
                                    if (WeaponInfo[j] == 2)
                                    {
                                        switch (WeaponType[j])
                                        {
                                        case 1:
                                            EquipedWeapon[i].Bow = true;
                                            Damage[i] += RangeWeapon.Bow;
                                            //AttackDelay[i] = RangeWeapon.Bow;
                                            break;
                                        case 2:
                                            EquipedWeapon[i].ThrKnife = true;
                                            Damage[i] += RangeWeapon.ThrKnife;
                                            //AttackDelay[i] = RangeWeapon.ThrKnife;
                                            break;
                                        case 3:
                                            EquipedWeapon[i].ThrAxe = true;
                                            Damage[i] += RangeWeapon.ThrAxe;
                                            //AttackDelay[i] = RangeWeapon.ThrAxe;
                                            break;
                                        case 4:
                                            EquipedWeapon[i].ThrSpear = true;
                                            Damage[i] += RangeWeapon.ThrSpear;
                                            //AttackDelay[i] = RangeWeapon.ThrSpear;
                                            break;
                                        case 5:
                                            EquipedWeapon[i].ThrTrident = true;
                                            Damage[i] += RangeWeapon.ThrTrident;
                                            //AttackDelay[i] = RangeWeapon.ThrTrident;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    for (int j = 0; j < NumberOfMutts; j++)
                    {
                        if (MuttPosX[j] == PosX[i] && MuttPosY[j] == PosY[i])
                        {
                            NewPosX[i] = PosX[i];
                            NewPosY[i] = PosY[i];

                            if (MuttAttack[j] >= MuttAttackDelay[j])
                            {
                                if (Dodgechance[i] > rand()%100)
                                {
                                    Health[i] += HealthRegeneration[i];
                                }
                                else
                                {
                                    Health[i] -= MuttDamage;
                                }
                                MuttAttack[j] = 0;
                            }

                            if (Attack[i] >= AttackDelay[i])
                            {
                                if (CritChance[i] > rand()%100)
                                {
                                    Health[i] -= Damage[i]*2;
                                }
                                else
                                {
                                    MuttHealth[j] -= Damage[i];
                                }
                                Attack[i] = 0;
                            }

                            MuttAttack[j]++;
                            Attack[i]++;

                            if (MuttHealth[j] <= 0)
                            {
                                MuttStatus[j] = 0;
                            }

                            if (Health[i] <= 0)
                            {
                                if (EnableScream)
                                    al_play_sample(SoundDeath[GenderType[i]][rand()%DeathNum[GenderType[i]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                                DeathText[i] = FirstName[i] + " was killed by a Mutt";
                                Health[i] = 0;
                                Status[i] = 0;

                            }
                        }
                    }


                    ///TRACKER JACKERS
                    for (int j = 0; j < NumberOfTrackerJackerNests; j++)
                    {
                        if (pow((NewPosX[i] - TrackerJackerNestX[j]),2) + pow((PosY[i] - TrackerJackerNestY[j]),2) <= pow(TrackerJackerNestR[j],2) && Status[i] != 0 && SurvivalSkill[i] > 5)
                            Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                        for (int k = 0; k < NumberOfTrackerJackers; k++)
                        {
                            if (TrackerJackerX[j][k] == PosX[i] && TrackerJackerY[j][k] == PosY[i])
                            {
                                Health[i] -= TrackerJackerDamage;
                                //WaterNeed[i] -= TrackerJackerDamage/2;
                                //FoodNeed[i] -= TrackerJackerDamage/2;

                                NewPosX[i] = PosX[i];
                                NewPosY[i] = PosY[i];
                                if (Health[i] <= 0)
                                {
                                    al_play_sample(Tracker_Jackers,0.13,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                                    if (EnableScream)
                                        al_play_sample(SoundDeath[GenderType[i]][rand()%DeathNum[GenderType[i]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                                    DeathText[i] = FirstName[i] + " was stung to death by Tracker Jackers";
                                    Health[i] = 0;
                                    Status[i] = 0;

                                }
                            }
                        }
                    }

                    ///FIRE
                    for (int j = 0; j < NumberOfFire; j++)
                    {
                        if (pow((FireX[j] - NewPosX[i]),2) + pow((FireY[j] - NewPosY[i]),2) <= pow(FireR,2))
                        {
                            Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                        }

                        if (FireDuration != FireOnTribute[i] && FireBurning[i])
                            FireOnTribute[i]++;

                        if (FireOnTribute[i] == FireDuration)
                            FireBurning[i] = false;

                        if (pow((FireX[j] - PosX[i]),2) + pow((FireY[j] - PosY[i]),2) <= pow(FireR,2))
                        {
                            FireBurning[i] = true;
                            FireOnTribute[i] = 0;
                        }

                        for (int k = 0; k < NumberOfWeapons; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],WeaponX[k],WeaponY[k],FireR);

                        for (int k = 0; k < NUMBER_OF_TREES; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],LeafX[k],LeafY[k],FireR);

                        for (int k = 0; k < NumberOfBackpacks; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],BackpackX[k],BackpackY[k],FireR);

                        for (int k = 0; k < NumberOfMedpacks; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],MedpackPosX[k],MedpackPosY[k],FireR);

                        for (int k = 0; k < NumberOfFood; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],FoodPosX[k],FoodPosY[k],FireR);

                        for (int k = 0; k < NumberOfNightlocks; k++)
                            CheckIfObjectIsInsideFire(FireX[j],FireY[j],NightlockPosX[k],NightlockPosY[k],FireR);

                        if (FireBurning[i])
                        {

                            if (SurvivalSkill[i] < 9)
                                Health[i] -= FireDamage;
                            else
                                Health[i] -= FireDamage/2;

                            if (Health[i] <= 0)
                            {
                                if (EnableScream)
                                    al_play_sample(SoundDeath[GenderType[i]][rand()%DeathNum[GenderType[i]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                                DeathText[i] = FirstName[i] + " burned to death";
                                Health[i] = 0;
                                Status[i] = 0;

                            }
                        }
                    }

                    ///FLOOD
                    for (int j = 0; j < NUMBER_OF_PONDS; j++)
                    {
                        for (int k = 0; k < NUMBER_OF_PONDS_INSIDE_PONDS; k++)
                        {
                            if (EventText == "Event: Flood")
                            {
                                if (pow((PondX[j] - NewPosX[i]),2) + pow((PondY[j] - NewPosY[i]),2) <= pow(PondR[j]/1.5,2) ||
                                        pow((ExtraPondX[j][k] - NewPosX[i]),2) + pow((ExtraPondY[j][k] - NewPosY[i]),2) <= pow(ExtraPondR[j][k]/1.5,2))
                                {
                                    Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                                }

                                if (pow((PondX[j] - PosX[i]),2) + pow((PondY[j] - PosY[i]),2) <= pow(PondR[j]/1.5,2) ||
                                        pow((ExtraPondX[j][k] - PosX[i]),2) + pow((ExtraPondY[j][k] - PosY[i]),2) <= pow(ExtraPondR[j][k]/1.5,2))
                                {
                                    if (SurvivalSkill[i] < 9)
                                        Health[i] -= 1;
                                    else
                                        Health[i] -= 0.5;
                                }

                                if (Health[i] <= 0)
                                {
                                    DeathText[i] = FirstName[i] + " drowned";
                                    Health[i] = 0;
                                    Status[i] = 0;

                                }
                            }
                            for (int l = 0; l < NumberOfWeapons; l++)
                                CheckIfObjectIsInsideWater(PondX[j],PondY[j],PondR[j],ExtraPondX[j][k],ExtraPondY[j][k],ExtraPondR[j][k],WeaponX[l],WeaponY[l]);

                            for (int l = 0; l < NumberOfBackpacks; l++)
                                CheckIfObjectIsInsideWater(PondX[j],PondY[j],PondR[j],ExtraPondX[j][k],ExtraPondY[j][k],ExtraPondR[j][k],BackpackX[l],BackpackY[l]);

                            for (int l = 0; l < NumberOfMedpacks; l++)
                                CheckIfObjectIsInsideWater(PondX[j],PondY[j],PondR[j],ExtraPondX[j][k],ExtraPondY[j][k],ExtraPondR[j][k],MedpackPosX[l],MedpackPosY[l]);

                            for (int l = 0; l < NumberOfFood; l++)
                                CheckIfObjectIsInsideWater(PondX[j],PondY[j],PondR[j],ExtraPondX[j][k],ExtraPondY[j][k],ExtraPondR[j][k],FoodPosX[l],FoodPosY[l]);

                            for (int l = 0; l < NumberOfNightlocks; l++)
                                CheckIfObjectIsInsideWater(PondX[j],PondY[j],PondR[j],ExtraPondX[j][k],ExtraPondY[j][k],ExtraPondR[j][k],NightlockPosX[l],NightlockPosY[l]);
                        }
                    }

                    ///BORDER
                    if (ArenaShrinking || ArenaShrunk)
                    {
Recheckpos:
                        if (pow((MainDisp/2 - NewPosX[i]),2) + pow((MainDisp/2 - NewPosY[i]),2) >= pow(MainDisp/2 - ArSi,2))
                        {
                            Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                            goto Recheckpos;
                        }

                        if (pow((MainDisp/2 - PosX[i]),2) + pow((MainDisp/2 - PosY[i]),2) >= pow(MainDisp/2 - ArSi,2))
                        {
                            Health[i] -= BorderDamage;
                        }

                        if (Health[i] <= 0)
                        {
                            if (EnableScream)
                                al_play_sample(SoundDeath[GenderType[i]][rand()%DeathNum[GenderType[i]]+1],0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);

                            Health[i] = 0;
                            Status[i] = 0;
                            DeathText[i] = FirstName[i] + " was electrocuted by the border";
                        }
                    }

                    ///SPONSORS
                    if (rand()%100 <= 0 && Day >= 1)
                    {
                        if (rand()%1000 <= 0)
                        {
                            Inventory[i].Food++;
                            al_play_sample(Sponsor,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        }
                        if (rand()%1000 <= 0)
                        {
                            Inventory[i].Water++;
                            al_play_sample(Sponsor,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        }
                        if (rand()%10000 <= 0)
                        {
                            Inventory[i].Medpack++;
                            al_play_sample(Sponsor,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        }
                    }

                    ///Consumptions
                    if (FoodNeed[i] < 50 && Inventory[i].Food > 0)
                    {
                        Inventory[i].Food--;
                        FoodNeed[i] += 50;
                    }
                    if (FoodNeed[i] < 50 && Inventory[i].Nightlock > 0 && SurvivalSkill[i] < 7)
                    {
                        Inventory[i].Nightlock--;
                        Status[i] = 0;
                        Health[i] = 0;
                        DeathText[i] = FirstName[i] + " unwittingly eats toxic berries";
                    }
                    if (FoodNeed[i] >= MaxOnDets/1.3 && WaterNeed[i] >= MaxOnDets/1.3)
                    {
                        Health[i] += HealthRegeneration[i];
                    }
                    if (FoodNeed[i] <= 0)
                    {
                        FoodNeed[i] = 0;
                        Health[i] -= HealthDeterioration[i];
                    }
                    if (WaterNeed[i] <= 0)
                    {
                        WaterNeed[i] = 0;
                        Health[i] -= HealthDeterioration[i];
                    }
                    if (WaterNeed[i] < MaxOnDets/2 && Inventory[i].Water > 0)
                    {
                        Inventory[i].Water--;
                        WaterNeed[i] += 50;
                    }
                    if (WaterNeed <= 0)
                    {
                        WaterNeed[i] = 0;
                        Health[i] -= HealthDeterioration[i];
                    }
                    if (Health[i] < MaxOnDets/2 && Inventory[i].Medpack > 0 && FoodNeed[i] > 0 && WaterNeed[i] > 0)
                    {
                        Inventory[i].Medpack--;

                        if (SurvivalSkill[i] > 5)
                            Health[i] += 50;
                        else
                            Health[i] += 25;
                    }
                    if (Health[i] <= 0)
                    {
                        Status[i] = 0;
                        Health[i] = 0;

                        if (WaterNeed[i] <= 0 && FoodNeed[i] <= 0)
                            DeathText[i] = FirstName[i] + " died from dehydration and starvation";
                        else if (WaterNeed[i] <= 0)
                            DeathText[i] = FirstName[i] + " died from dehydration";
                        else if (FoodNeed[i] <= 0)
                            DeathText[i] = FirstName[i] + " died from starvation";
                    }
                    if (Health[i] >= MaxOnDets)
                    {
                        Health[i] = MaxOnDets;
                    }
                    if (FoodNeed[i] >= MaxOnDets)
                    {
                        FoodNeed[i] = MaxOnDets;
                    }
                    if(WaterNeed[i] >= MaxOnDets)
                    {
                        WaterNeed[i] = MaxOnDets;
                    }

                    FoodNeed[i] -= HungerDeteriorationRate[i];
                    WaterNeed[i] -= WaterDeteriorationRate[i];

                    if ((Grid[PosX[i]+1][PosY[i]] == 1)
                            || (Grid[PosX[i]-1][PosY[i]] == 1)
                            || (Grid[PosX[i]][PosY[i]+1] == 1)
                            || (Grid[PosX[i]][PosY[i]-1] == 1) || (sqrt(pow((PosX[i]-MainDisp/2),2)+pow((PosY[i]-MainDisp/2),2)) >= MainDisp/2))
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);

                    ////Day 1 Event
                    if (Day < 0)
                    {
                        if (Organization[i] == 'J' && (pow((NewPosX[i] - MainDisp/2),2) + pow((NewPosY[i] - MainDisp/2),2) <= pow(MainDisp/5,2)))
                        {
                            Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                        }
                        else if (Organization[i] == 'P' && (pow((NewPosX[i] - MainDisp/2),2) + pow((NewPosY[i] - MainDisp/2),2) >= pow(MainDisp/5,2)))
                        {
                            Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                        }
                    }

                    //if (Decision[i] == 'T')
                    //if (Energy[i] == 'I' && Information[i] == 'N' && Decision[i] == 'T'  && Organization[i] == 'J')
                    //    al_draw_circle(PosX[i],PosY[i],3,WHITE,1);

                    if (!ArenaShrunk)
                        for (int j = 0; j < NumberOfPlayers; j++)
                        {
ReWander:
                            if (Day > 1 && Day < 3 && pow((PosX[j] - NewPosX[i]),2) + pow((PosY[j] - NewPosY[i]),2) <= pow(AREA_RADIUS,2) && j != i
                                    && PosX[i] != PosX[j] && PosY[i] != PosY[j] && Energy[i] == 'I' && Relations[i][j] <= 0)
                            {
                                Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                                goto ReWander;
                            }
                        }
                    if (ArenaShrunk || Flooded || Fired || MuttsReleased)
                    {
                        Memory[i].WaterX = -1;
                        Memory[i].WaterY = -1;
                    }

                    if ((NewPosX[i]!=PosX[i])||(NewPosY[i]!=PosY[i]))
                    {
                        if (rand()%50>0)
                        {
                            if (PosX[i]!= NewPosX[i])
                            {
                                if(PosX[i]<NewPosX[i] && Grid[PosX[i]+1][PosY[i]] != 1)
                                    PosX[i]++;
                                else if (PosX[i]>NewPosX[i] && Grid[PosX[i]-1][PosY[i]] != 1)
                                    PosX[i]--;
                            }
                            if (PosY[i]!= NewPosY[i])
                            {
                                if(PosY[i]<NewPosY[i] && Grid[PosX[i]][PosY[i]+1] != 1)
                                    PosY[i]++;
                                else if (PosY[i]>NewPosY[i] && Grid[PosX[i]][PosY[i]-1] != 1)
                                    PosY[i]--;
                            }
                        }
                    }

                    else
                        Wander(NewPosX[i],PosX[i],NewPosY[i],PosY[i],AREA_RADIUS,MainDisp);
                    break;
                }
            }

            ///EVENTS
            ///FEAST
            if (Day > 3 && !ArenaEvent && !Feasted)
            {
                int FoodOnArena = NumberOfFood;
                int MedpackOnArena = NumberOfMedpacks;

                for (int i = 0 ; i < NumberOfFood; i++)
                    if (FoodPosX[i] == -999 && FoodPosY[i] == -999)
                        FoodOnArena--;

                for (int i = 0; i < NumberOfMedpacks; i++)
                    if (MedpackPosX[i] == -999 && MedpackPosY[i] == -999)
                        MedpackOnArena--;

                if (FoodOnArena <= NumberOfFood/2 || MedpackOnArena <= MedpackOnArena/2)
                {
                    int N = 0;
                    for (int i = 0; i < NumberOfPlayers; i++)
                        N += Status[i];

                    if (FoodOnArena <= NumberOfFood/2)
                    {
                        int Placed = 0;
                        for (int j = 0 ; j < NumberOfFood; j++)
                        {
                            if (FoodPosX[j] == -999 && FoodPosY[j] == -999)
                            {
                                if (N <= Placed)
                                    break;
                                GenerateWeaponPos(FoodPosX[j],FoodPosY[j],MainDisp);
                                Placed++;
                            }
                        }
                    }

                    if (MedpackOnArena <= MedpackOnArena/2)
                    {
                        int Placed = 0;
                        for (int j = 0; j < NumberOfMedpacks; j++)
                        {
                            if (MedpackPosX[j] == -999 && MedpackPosY[j] == -999)
                            {
                                if (N <= Placed)
                                    break;
                                GenerateWeaponPos(MedpackPosX[j],MedpackPosY[j],MainDisp);
                                Placed++;
                            }
                        }
                    }

                    for (int i = 0; i < NumberOfPlayers; i++)
                    {
                        if (Inventory[i].Food <= 0 || Inventory[i].Medpack <= 0)
                        {
                            NewPosX[i] = MainDisp/2;
                            NewPosY[i] = MainDisp/2;
                        }
                    }
                    ArenaEvent = true;
                    Feasted = true;
                    EventText = "Event: Feast";
                }
            }


            ///MUTTS
            if ((Day > 2 && !ArenaEvent && !MuttsReleased && AlivePlayers >= 2) || (MuttsAlive && !MuttsReleased && AlivePlayers >= 2)) ///Day > 2
            {
                int odds = 0;
                int y = rand()%1000; ///100 <- Too Small && Too High -> 10000 ~ 1000 <- Perfect (Tho still too high)

                if (y <= odds || MuttsAlive)
                {
                    MuttsAlive = true;
                    ArenaEvent = true;
                    EventText = "Event: Mutts";

                    if (!Counted && AlivePlayers > 2)
                    {
                        al_play_sample(MuttSpawnSound,0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        CountedPlayers = 0;
                        for (int i = 0; i < NumberOfPlayers; i++)
                            if(Status[i] != 0)
                            {
                                CountedPlayers++;
                                Counted=true;
                            }
                    }

                    for (int i = 0; i < NumberOfMutts; i++)
                    {
                        switch (MuttStatus[i])
                        {
                        case 1:
                        {
regenalpha:
                            if (!MuttStatus[MuttAlpha])
                            {
                                MuttAlpha = rand()%NumberOfMutts;
                                goto regenalpha;
                            }

                            al_draw_filled_circle(MuttPosX[i],MuttPosY[i],SizeofChar,MuttColor);
                            //al_draw_circle(MuttPosX[i],MuttPosY[i],MuttSight,Color,1);

                            if (MuttPosX[i] == MuttNewPosX[i] && MuttPosY[i] == MuttNewPosY[i])
                                Wander(MuttNewPosX[i],MuttPosX[MuttAlpha],MuttNewPosY[i],MuttPosY[MuttAlpha],MuttSight,MainDisp);

                            int Target = 0;
                            for (int j = 0; j < NumberOfPlayers; j++)
                            {
                                if (pow((PosX[j] - MuttPosX[i]),2) + pow((PosY[j] - MuttPosY[i]),2) <= pow(MuttSight,2) && Status[j] == 1)
                                {
                                    Target = j;
                                    for (int k = 0; k < NumberOfPlayers; k++)
                                    {
                                        if (pow((PosX[k] - MuttPosX[i]),2) + pow((PosY[k] - MuttPosY[i]),2) <= pow(MuttSight,2) && j != k)
                                        {
                                            if (sqrt((pow(MuttPosX[i]-PosX[Target],2)+pow(MuttPosY[i]-PosY[Target],2)))
                                                    >= sqrt((pow(MuttPosX[i]-PosX[k],2)+pow(MuttPosY[i]-PosY[k],2))) && Status[k] == 1)
                                            {
                                                Target = k;
                                            }
                                        }
                                    }
                                    MuttNewPosX[i] = PosX[Target];
                                    MuttNewPosY[i] = PosY[Target];
                                }
                            }

                            if (AlivePlayers <= (CountedPlayers/2) || AlivePlayers <= 2)
                            {
                                if (pow((MuttPosX[i] - MainDisp/2),2) + pow((MuttPosY[i] - MainDisp/2),2) >= pow(MainDisp/2.1,2))
                                {
                                    MuttStatus[i] = 0;
                                    break;
                                }

                                for (int j = 0; j < NumberOfMutts; j++)
                                {
                                    MuttNewPosX[j] = MuttHomeX;
                                    MuttNewPosY[j] = MuttHomeY;
                                }
                            }

                            if ((MuttNewPosX[i]!= MuttPosX[i])||(MuttNewPosY[i]!= MuttPosY[i]))
                            {
                                {
                                    if (MuttPosX[i]!= MuttNewPosX[i])
                                    {
                                        if(MuttPosX[i]<MuttNewPosX[i])
                                            MuttPosX[i] += MuttSpeed;
                                        else if (MuttPosX[i]>MuttNewPosX[i])
                                            MuttPosX[i] -= MuttSpeed;
                                    }
                                    if (MuttPosY[i]!= MuttNewPosY[i])
                                    {
                                        if(MuttPosY[i]<MuttNewPosY[i])
                                            MuttPosY[i] += MuttSpeed;
                                        else if (MuttPosY[i]>MuttNewPosY[i])
                                            MuttPosY[i] -= MuttSpeed;
                                    }
                                }
                            }
                            else
                                Wander(MuttNewPosX[i],MuttPosX[MuttAlpha],MuttNewPosY[i],MuttPosY[MuttAlpha],MuttSight,MainDisp);

                            break;
                        }
                        case 0:
                        {
                            MuttPosX[i] = -999;
                            MuttPosY[i] = -999;

                            int DeadMutts = 0;

                            for (int j = 0; j < NumberOfMutts; j++)
                                if (MuttStatus[j] != 1)
                                    DeadMutts++;

                            if (DeadMutts >= NumberOfMutts)
                            {
                                EventText = "Event: None";
                                ArenaEvent = false;
                                MuttsReleased = true;
                                Counted = false;
                                break;
                            }
                        }
                        break;
                        }
                    }
                }
            }

            if (Day > 2 && !Flooded && !ArenaEvent || Flooding)
            {
                int odds = 0;
                int y = rand()%1000; ///100 <- Too Small && Too High -> 10000 ~ 1000 <- Perfect (Tho still too high)

                if (y <= odds || Flooding)
                {
                    ArenaEvent = true;
                    Flooding = true;
                    EventText = "Event: Flood";

                    if (!Counted)
                    {
                        al_play_sample(Water_Start,0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        Counted = true;
                    }

                    if (EventTime > 0)
                    {
                        if (FloodSize != FloodMax)
                        {
                            FloodSize++;

                            for (int i = 0; i < NUMBER_OF_PONDS; i++)
                            {
                                PondR[i]++;
                                for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
                                    ExtraPondR[i][j]++;
                            }
                        }
                    }

                    EventTime -= DayIncrementation;

                    if (EventTime <= 0)
                    {
                        if (FloodSize != 0)
                        {
                            if(SoundEventPlayed)
                            {
                                al_play_sample(Water_End,0.3,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                                SoundEventPlayed = false;
                            }
                            al_stop_sample(&SoundEvent);
                            FloodSize--;
                            for (int i = 0; i < NUMBER_OF_PONDS; i++)
                            {
                                PondR[i]--;
                                for (int j = 0; j < NUMBER_OF_PONDS_INSIDE_PONDS; j++)
                                    ExtraPondR[i][j]--;
                            }
                        }
                    }
                    if (FloodSize >= FloodMax && !SoundEventPlayed)
                    {
                        al_play_sample(Water_Loop,0.3,0,1,ALLEGRO_PLAYMODE_LOOP,&SoundEvent);
                        SoundEventPlayed = true;
                    }

                    if (FloodSize <= 0)
                    {
                        EventText = "Event: None";
                        ArenaEvent = false;
                        Counted = false;
                        Flooded = true;
                        Flooding = false;
                        EventTime = 1;
                    }
                }
            }


            if (EventText == "Event: Feast")
            {
                EventTime -= DayIncrementation;
                if (EventTime <= 0)
                {
                    EventText = "Event: None";
                    ArenaEvent = false;
                    EventTime = 1;
                }
            }

            for (int i = 0; i < NumberOfTrackerJackerNests; i++)
            {
                for (int j = 0; j < NumberOfTrackerJackers; j++)
                {
                    al_draw_filled_circle(TrackerJackerX[i][j],TrackerJackerY[i][j],SizeofChar/2,TrackerJackers);

                    if (TrackerJackerNewX[i][j] == TrackerJackerX[i][j] || TrackerJackerNewY[i][j] == PosY[i])
                        Wander(TrackerJackerNewX[i][j],TrackerJackerNestX[i],TrackerJackerNewY[i][j],TrackerJackerNestY[i],TrackerJackerNestR[i],MainDisp);

                    if ((TrackerJackerNewX[i][j]!=TrackerJackerX[i][j])||(TrackerJackerNewY[i][j]!=PosY[i]))
                    {
                        {
                            if (TrackerJackerX[i][j]!= TrackerJackerNewX[i][j])
                            {
                                if(TrackerJackerX[i][j]<TrackerJackerNewX[i][j])
                                    TrackerJackerX[i][j]++;
                                else if (TrackerJackerX[i][j]>TrackerJackerNewX[i][j])
                                    TrackerJackerX[i][j]--;
                            }
                            if (TrackerJackerY[i][j]!= TrackerJackerNewY[i][j])
                            {
                                if(TrackerJackerY[i][j]<TrackerJackerNewY[i][j])
                                    TrackerJackerY[i][j]++;
                                else if (TrackerJackerY[i][j]>TrackerJackerNewY[i][j])
                                    TrackerJackerY[i][j]--;
                            }
                        }
                    }
                    else
                        Wander(TrackerJackerNewX[i][j],TrackerJackerNestX[i],TrackerJackerNewY[i][j],TrackerJackerNestY[i],TrackerJackerNestR[i],MainDisp);
                }

                if (TrackerJackerNestR[i] != TrackerJackerNestDef)
                    TrackerJackerNestR[i]-=0.5;

                for (int j = 0; j < NumberOfPlayers; j++)
                {
                    if (pow((PosX[j] - TrackerJackerNestX[i]),2) + pow((PosY[j] - TrackerJackerNestY[i]),2) <= pow(TrackerJackerNestR[i],2)
                            && Status[j] != 0)
                    {
                        TrackerJackerNestR[i] = SizeofChar*40;

                        int Target = 0;
                        if (pow((PosX[j] - TrackerJackerNestX[i]),2) + pow((PosY[j] - TrackerJackerNestY[i]),2) <= pow(TrackerJackerNestR[i],2)
                                && Status[j] == 1)
                        {
                            Target = j;
                            for (int k = 0; k < NumberOfPlayers; k++)
                            {
                                if (pow((PosX[k] - TrackerJackerNestX[i]),2) + pow((PosY[k] - TrackerJackerNestY[i]),2) <= pow(TrackerJackerNestR[i],2)
                                        && j != k)
                                {
                                    if (sqrt((pow(TrackerJackerNestX[i]-PosX[Target],2)+pow(TrackerJackerNestY[i]-PosY[Target],2)))
                                            >= sqrt((pow(TrackerJackerNestX[i]-PosX[k],2)+pow(TrackerJackerNestY[i]-PosY[k],2))) && Status[k] == 1)
                                    {
                                        Target = k;
                                    }
                                }
                            }
                            for (int k = 0; k < NumberOfTrackerJackers; k++)
                            {
                                TrackerJackerNewX[i][k] = PosX[Target];
                                TrackerJackerNewY[i][k] = PosY[Target];
                            }
                        }
                    }
                }
            }

            if (Day > 2 && !ArenaEvent && !Fired || Firing)
            {
                int odds = 0;
                int y = rand()%1000;

                if (odds >= y || Firing)
                {
                    Firing = true;
                    ArenaEvent = true;
                    Fired = true;
                    EventText = "Event: Fire";

                    if (!Counted)
                    {
                        al_play_sample(Fire_Start,0.5,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        CountedPlayers = 0;
                        for (int i = 0; i < NumberOfPlayers; i++)
                            if(Status[i] != 0)
                            {
                                CountedPlayers++;
                                Counted = true;
                            }
                    }

                    for (int i = 0; i < NumberOfFire; i++)
                        al_draw_filled_circle(FireX[i],FireY[i],FireR,FIRE);

                    EventTime -= DayIncrementation;
                    if (EventTime <= 0 || AlivePlayers <= CountedPlayers/2)
                    {
                        if (SoundEventPlayed)
                        {
                            al_play_sample(Fire_End,0.5,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                            SoundEventPlayed = false;
                        }

                        al_stop_sample(&SoundEvent);

                        FireR--;
                        if (FireR <= 0)
                        {
                            EventText = "Event: None";
                            ArenaEvent = false;
                            Firing = false;
                            Counted = false;
                            EventTime = 1;
                        }
                    }
                    else if (FireR != FireRMax)
                        FireR++;
                    if (FireR == FireRMax && !SoundEventPlayed)
                    {
                        al_play_sample(Fire_Loop,0.5,0,1,ALLEGRO_PLAYMODE_LOOP,&SoundEvent);
                        SoundEventPlayed = true;
                    }
                }
            }

            for (int i = 0; i < NumberOfFood; i++)
                PlaceFood(SizeofChar,NumberOfFood,FoodPosX[i],FoodPosY[i]);

            for (int i = 0; i < NumberOfWeapons; i++)
                PlaceWeapons(WeaponX[i],WeaponY[i],WeaponR[i],WeaponInfo[i]);

            for (int i = 0; i < NumberOfBackpacks; i++)
                PlaceBackpack(BackpackX[i],BackpackY[i],BackpackR);

            for (int i = 0; i < NumberOfMedpacks; i++)
                PlaceMedpack(MedpackPosX[i],MedpackPosY[i],MedpackPosR);

            for (int i = 0; i < NumberOfNightlocks; i++)
                PlaceNightlock(NightlockPosX[i],NightlockPosY[i],SizeofChar);

            for(int i = 0; i < NUMBER_OF_TREES; i++)
                //Environment
            {
                PrintEnvironmentTrees(TreeX[i],TreeY[i],TreeThickness[i]);
                PrintEnvironmentLeaves(LeafX[i],LeafY[i],LeafR[i]);
            }

            ///ARENA SHINKING
            if (Day > 1 && AlivePlayers <= NumberOfPlayers/2 && !ArenaEvent && !ArenaShrunk || ArenaShrinking)
            {
                int odds = 0;
                int y = rand()%1000000;

                if (odds >= y || ArenaShrinking || (AlivePlayers <= 2 && Day >= 10))
                {
                    if (!Counted)
                    {
                        al_play_sample(ArenaShrinkingound,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                        Counted = true;
                    }

                    EventText = "Event: Arena Shrinking";
                    ArenaShrinking = true;
                    ArenaEvent = true;

                    if (ArSi != MainDisp/4)
                        ArSi++;

                    else
                    {
                        ArenaShrunk = true;
                        EventText = "Event: None";
                    }
                }
            }

            DeadPlayers = 0;
            for (int i = 0; i < NumberOfPlayers; i++)
            {
                if (Status[i] == 0)
                    DeadPlayers++;
            }

            if (DeadPlayers != PlayedCannons)
            {
                CannonIncrement++;
            }

            if (Day > 1 && PlayedCannons == DeadPlayers)
            {
                CannonDelay = 1;
            }

            if (CannonDelay <= CannonIncrement && Day > 1)
            {
                al_play_sample(Cannon,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                PlayedCannons++;
                CannonIncrement = 0;
            }

            al_draw_circle(MainDisp/2,MainDisp/2,MainDisp/2 - ArSi,WHITE,2);
            al_draw_circle(MainDisp/2,MainDisp/2,MainDisp,BLACK,MainDisp + ArSi*2);

        }

        if (!GameSimulation)
        {
            while(Paused2)
            {
                for (int i = 0; i < NumberOfPlayers; i++)
                {
                    Count[i] = to_string(KillCount[i]) + " Kill";
                    Placement[i] = to_string(NumberOfPlayers-(DeathOrder[i]-1)) + ordinal_suffix(NumberOfPlayers-(DeathOrder[i]-1)) + " Place";

                    if (KillCount[i] > 1)
                        Count[i] += "s";

                    if (DeathOrder[i] == 0)
                    {
                        Placement[i] = "Victor";
                        DeathText[i] = "Victor";
                    }
                }

                al_stop_timer(timer);
                if (!Printed)
                {
                    al_draw_text(ROBOTO_BOLD,WHITE,MainDisp/2,MainDisp/2,ALLEGRO_ALIGN_CENTRE,"[Press anywhere to continue]");
                    Printed = true;
                }

                al_register_event_source(MouseQue, al_get_display_event_source(MainDisplay));
                al_register_event_source(MouseQue, al_get_display_event_source(SecondDisplay));
                al_register_event_source(MouseQue, al_get_display_event_source(ThirdDisplay));

                //To Close
                al_wait_for_event(MouseQue, &EventMouse);
                if(EventMouse.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    exit(0);

                al_flip_display();

                al_get_mouse_state(&state);
                if (state.buttons & 1)
                    if(EventMouse.mouse.button)
                        if (EventMouse.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                        {
                            Printed = false;
                            Paused2 = false;
                            al_start_timer(timer);
                            al_clear_to_color(BLACK);
                            al_stop_samples();
                        }
            }

            if (!Played)
            {
                al_play_sample(The_Horn_Of_Plenty,0.5,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                Played = true;

                SetNewYear(Number);
                if (EnableSaveFile)
                {
                    ofstream AFile("Tribute Files/Previous Games.txt",ios::app);
                    AFile << "\n==============================\n" << TITLE << "\n";

                    for (int i = NumberOfPlayers-1; i >= 0; i--)
                    {
                        for (int j = 0; j < NumberOfPlayers; j++)
                        {
                            if (DeathOrder[j] == 0 && i+1 == NumberOfPlayers)
                            {
                                AFile << "\n" << Dist[j] << "\n" << FullName[j] << "\n" << Placement[j]  << "\n";
                            }

                            if (DeathOrder[j] == i+1)
                            {
                                AFile << "\n" << Dist[j] << "\n" << FullName[j] << "\n" << Placement[j]  << "\n" << DeathText[j] << "\n";
                            }
                        }
                    }
                    AFile.close();

                    for (int i = 0; i < NumberOfPlayers; i++)
                        if (DeathOrder[i] == 0)
                        {
                            ofstream MyFile("Tribute Files/List of Victors.txt",ios::app);
                            MyFile << "\n" << TITLE <<  ":\n" << FullName[i] << "\n" << Dist[i] << "\n";
                            MyFile.close();
                        }
                }
            }

            Seconds++;

            Printed = false;

            if (!TheFallenDone)
            {
                for (int i = NumberOfPlayers-1; i >= 0; i--)
                {
                    for (int j = 0; j < NumberOfPlayers; j++)
                    {
                        if (DeathOrder[j] == i+1 && (Seconds/FPS) == SongLength/24*(i+1) && !Printed && Seconds/FPS != Proxy ||
                                DeathOrder[j] == 0 && (Seconds/FPS) == SongLength/24*24 && !Printed && Seconds/FPS != Proxy)
                        {
                            al_clear_to_color(BLACK);
                            Proxy = Seconds/FPS;
                            Printed = true;

                            for (int k = 0; k < 5; k++)
                            {
                                al_draw_text(DISTRICT,al_map_rgb((k+1)*20,(k+1)*20,(k+1)*20),MainDisp/2,MainDisp/5-FontSize+(50*k),ALLEGRO_ALIGN_CENTRE,Dist[j].c_str());
                            }

                            al_draw_tinted_scaled_bitmap(Head[j],SKIN_COLOR[j],0,0,500,500,(MainDisp/2)-150,(MainDisp/2.20)-150,300,300,NULL);
                            al_draw_tinted_scaled_bitmap(Body[j],TRIBUTE_COLOR[j],0,0,500,500,(MainDisp/2)-150,(MainDisp/2.20)-150,300,300,NULL);
                            al_draw_tinted_scaled_bitmap(Hair[j],HAIR_COLOR[j],0,0,500,500,(MainDisp/2)-150,(MainDisp/2.20)-150,300,300,NULL);

                            //al_draw_text(ROBOTO_BOLD,WHITE,MainDisp/2,MainDisp/3,ALLEGRO_ALIGN_CENTRE,Placement[j].c_str());
                            al_draw_filled_rounded_rectangle(MainDisp/7,MainDisp-MainDisp/2.90,MainDisp-MainDisp/7,MainDisp-MainDisp/3.90,10,10,al_map_rgb(40,40,40));
                            al_draw_text(NAME,WHITE,MainDisp/2,MainDisp-MainDisp/3,ALLEGRO_ALIGN_CENTRE,FullName[j].c_str());
                            al_draw_text(ROBOTO_REGULAR,WHITE,MainDisp/2,MainDisp-MainDisp/3+FontSize*1.9,ALLEGRO_ALIGN_CENTRE,DeathText[j].c_str());
                        }
                    }
                }
            }

            if (Seconds/FPS >= SongLength && !TheFallenDone)
            {
                TheFallenDone = true;
                al_clear_to_color(BLACK);
                //al_draw_filled_rounded_rectangle(MainDisp/8,MainDisp/8,MainDisp-MainDisp/8,MainDisp-MainDisp/8,20,20,al_map_rgb(40,40,40));
                //al_draw_filled_circle(MainDisp/2,MainDisp/2.15,MainDisp/3,BLACK);
                al_draw_scaled_bitmap(Capitol_Seal,0,0,800,800,MainDisp/5,MainDisp/6,317,317,NULL);
                al_draw_tinted_scaled_bitmap(Capitol_Seal,al_map_rgba(255,215,0,0),0,0,800,800,MainDisp/5,MainDisp/6,317,317,NULL);
                al_draw_text(CAPITOL,WHITE,MainDisp/2,MainDisp-MainDisp/4.5,ALLEGRO_ALIGN_CENTRE,"BROUGHT TO YOU BY THE CAPITOL");
            }

            if (TheFallenDone)
                PostGameStatistics = true;
        }
        //GameSimulation = false;

        al_flip_display();
        //End of Main Screen

        al_set_target_backbuffer(SecondDisplay);
        //Second Display
        if (!GameSimulation && !Capitol_Seal_Printed)
        {
            al_clear_to_color(BLACK);
            al_draw_scaled_bitmap(Capitol_Seal,0,0,800,800,SecDispV/7.6,SecDispH/4,200,200,NULL);
            al_draw_tinted_scaled_bitmap(Capitol_Seal,al_map_rgba(255,215,0,0),0,0,800,800,SecDispV/7.6,SecDispH/4,200,200,NULL);
            al_draw_text(CAPITOL,WHITE,SecDispV/2,SecDispH-SecDispH/5,ALLEGRO_ALIGN_CENTRE,"THE FALLEN");
            al_flip_display();
            Capitol_Seal_Printed = true;
        }

        int TextX = FontSize;
        int TextY = 10;

        int N = 0;
        for (int i = 0; i < NumberOfPlayers; i++)
            N += Status[i];

        string Players = to_string(N);
        string Word = to_string(int(Day));

        if (GameSimulation)
        {
            al_clear_to_color(BLACK);
            for (int i = 0; i < NumberOfPlayers; i++)
            {
                if(Status[i] == 0)
                    NAMECOLOR = GREY;
                else if (Status[i] == 1)
                    NAMECOLOR = TRIBUTE_COLOR[i];

                ALLEGRO_COLOR BG = al_map_rgb(20,20,20);

                if (Status[i] == 0)
                    BG = BLACK;

                al_draw_filled_rectangle(TextX-FontSize/10,TextY,TextX+MaxOnDets+FontSize/10,TextY+FontSize*1.8,BG);

                al_draw_line(TextX,TextY+FontSize,TextX+Health[i],TextY+FontSize,al_map_rgb(100,400,300),FontSize/4);

                al_draw_line(TextX,TextY+FontSize*1.4,TextX+WaterNeed[i],TextY+FontSize*1.4,al_map_rgb(0,100,200),FontSize/4);
                al_draw_line(TextX,TextY+FontSize*1.6,TextX+FoodNeed[i],TextY+FontSize*1.6,al_map_rgb(500,200,300),FontSize/4);

                al_draw_text(ROBOTO_REGULAR,NAMECOLOR,TextX,TextY,ALLEGRO_ALIGN_LEFT,FullName[i].c_str());


                //al_draw_text(ROBOTO_REGULAR,NAMECOLOR,TextX,TextY,ALLEGRO_ALIGN_LEFT,FirstName[i].c_str());
                TextY = TextY + FontSize/2;
                //al_draw_text(ROBOTO_BOLD,NAMECOLOR,TextX,TextY,ALLEGRO_ALIGN_LEFT,LastName[i].c_str());

                TextY = TextY - FontSize/2.5;
                TextX = TextX + SecDispV/2;

                if (i%2 != 0)
                {
                    TextX = FontSize;
                    TextY = TextY + FontSize + FontSize/1.1;
                }
            }

            Day += DayIncrementation;
            al_flip_display();
        }

        if (PostGameStatistics)
        {
            al_clear_to_color(BLACK);
            al_draw_text(ROBOTO_BOLD,WHITE,SecDispV/2,SecDispH/20,ALLEGRO_ALIGN_CENTRE,TITLE.c_str());
            for (int i = 0; i < NumberOfPlayers; i++)
            {
                switch (Status[i])
                {
                case 1:
                    al_draw_text(ROBOTO_REGULAR,WHITE,SecDispV/2,SecDispH/20+FontSize*2,ALLEGRO_ALIGN_CENTRE,FirstName[i].c_str());
                    al_draw_text(ROBOTO_BOLD,WHITE,SecDispV/2,SecDispH/20+FontSize*2+(FontSize/2),ALLEGRO_ALIGN_CENTRE,LastName[i].c_str());
                    al_draw_text(DAYCOUNTER,WHITE,SecDispV/2,(SecDispH/20+FontSize*2)+24,ALLEGRO_ALIGN_CENTRE,Dist[i].c_str());
                    break;
                }
            }
            Down2 = 1;
            Side = 0;
            for (int j= 0; j < NumberOfPlayers; j++)
            {
                al_draw_text(ROBOTO_REGULAR,WHITE,SecDispV/8 + (Side*SecDispV)/4,((SecDispH/6+FontSize*Down2*3)),ALLEGRO_ALIGN_CENTRE,FirstName[j].c_str());
                al_draw_text(DAYCOUNTER,WHITE,SecDispV/8 + (Side*SecDispV)/4,((SecDispH/6+FontSize*Down2*3)+(FontSize/1.5)),ALLEGRO_ALIGN_CENTRE,Dist[j].c_str());
                al_draw_text(DAYCOUNTER,WHITE,SecDispV/8 + (Side*SecDispV)/4,((SecDispH/6+FontSize*Down2*3)+16),ALLEGRO_ALIGN_CENTRE,Placement[j].c_str());
                if (KillCount[j] > 0)
                    al_draw_text(DAYCOUNTER,WHITE,SecDispV/8 + (Side*SecDispV)/4,((SecDispH/6+FontSize*Down2*3)+23),ALLEGRO_ALIGN_CENTRE,Count[j].c_str());
                Side++;
                switch ((j+1)%4)
                {
                case 0:
                    Down2++;
                    Side = 0;
                    break;
                }
            }
            PostGameStatistics = false;
            al_stop_timer(timer);
            al_flip_display();
        }
        //End of Second Display

        al_set_target_backbuffer(ThirdDisplay);
        //Third Display

        int Xline = SecDispV/10;
        int Yline = ThiDispH/6;

        int Xline2 = SecDispV/1.1;
        int Yline2 = ThiDispH/6;

        if (!PostGameStatistics && CountedDown)
        {
            al_clear_to_color(BLACK);
            al_draw_text(DAYCOUNTER,WHITE,SecDispV/2,ThiDispH/20,ALLEGRO_ALIGN_CENTRE,Players.c_str());
            al_draw_text(DAYCOUNTER,WHITE,SecDispV-40,ThiDispH/20,ALLEGRO_ALIGN_RIGHT,"Day:");
            al_draw_text(DAYCOUNTER,WHITE,SecDispV-40,ThiDispH/20,ALLEGRO_ALIGN_LEFT,Word.c_str());
            al_draw_text(DAYCOUNTER,WHITE,SecDispV/10,ThiDispH/20,ALLEGRO_ALIGN_LEFT,EventText.c_str());
            al_draw_line(Xline,Yline,Xline2,Yline2,WHITE,SizeofChar);

            for (int i = NumberOfPlayers; i > 0; i--)
                for (int j = 0; j < NumberOfPlayers; j++)
                {
                    if (DeathOrder[j] == i && Status[j] != 1)
                    {
                        al_draw_text(DAYCOUNTER,WHITE,Xline,Yline+4,ALLEGRO_ALIGN_LEFT,DeathText[j].c_str());
                        Yline += FontSize/2;
                    }
                }

            Yline2 = ThiDispH -  ThiDispH/6 + SizeofChar*10;
            Yline = ThiDispH -  ThiDispH/6 + SizeofChar*10;

            Xline2 = 0;
            Xline = SecDispV;
            al_draw_line(Xline,Yline,Xline2,Yline2,BLACK,SizeofChar*27);

            Yline2 = ThiDispH - ThiDispH/6;
            Yline = ThiDispH - ThiDispH/6;
            Xline = SecDispV/10;
            Xline2 = SecDispV/1.1;
            al_draw_line(Xline,Yline,Xline2,Yline2,WHITE,SizeofChar);
        }

        if (!GameSimulation && !PostGameStatistics && TheFallenDone && !Printed)
        {
            al_clear_to_color(BLACK);
            Printed = true;

            for (int i = 0; i <NumberOfPlayers; i++)
            {
                if (Status[i] != 0)
                {
                    al_draw_tinted_scaled_bitmap(Head[i],SKIN_COLOR[i],0,0,500,500,SecDispV/2-50,ThiDispH/2-50,100,100,NULL);
                    al_draw_tinted_scaled_bitmap(Body[i],TRIBUTE_COLOR[i],0,0,500,500,SecDispV/2-50,ThiDispH/2-50,100,100,NULL);
                    al_draw_tinted_scaled_bitmap(Hair[i],HAIR_COLOR[i],0,0,500,500,SecDispV/2-50,ThiDispH/2-50,100,100,NULL);

                    al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH-ThiDispH/5,ALLEGRO_ALIGN_CENTRE,FullName[i].c_str());
                    al_draw_text(DAYCOUNTER,WHITE,SecDispV/2,ThiDispH-ThiDispH/5-FontSize/2,ALLEGRO_ALIGN_CENTRE,Dist[i].c_str());
                }
            }

            al_draw_text(DAYCOUNTER,WHITE,SecDispV/2,ThiDispH/100,ALLEGRO_ALIGN_CENTRE," -= End of Simulation =- ");
        }

        if (!CountDown && GameSimulation && !CountedDown)
        {
            if (!PlayedCountdown)
            {
                al_play_sample(Countdown,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
                PlayedCountdown = true;
            }

            Seconds++;
            if(Seconds%30 == 0 && Seconds/FPS < CountdownLength)
            {
                al_clear_to_color(BLACK);
                al_draw_text(CAPITOL,WHITE,SecDispV/2,ThiDispH/2,ALLEGRO_ALIGN_CENTRE,to_string(abs((Seconds/FPS)-CountdownLength)).c_str());
            }

            if(Seconds/FPS >= CountdownLength)
            {
                Day = 0;
                CountDown = true;
                CountedDown = true;
                Seconds = 0;
            }
        }

        if (CountedDown)
            CountDown = true;

        al_flip_display();
        //End of Third Display

        al_register_event_source(Que, al_get_display_event_source(MainDisplay));
        al_register_event_source(Que, al_get_display_event_source(SecondDisplay));
        al_register_event_source(Que, al_get_display_event_source(ThirdDisplay));
        //To Close
        al_wait_for_event(Que, &Event);
        if(Event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
    }

    //Destruction of Allegro
    al_destroy_event_queue(Que);
    al_destroy_event_queue(MouseQue);
    al_destroy_display(MainDisplay);
    al_destroy_display(SecondDisplay);
    al_destroy_display(ThirdDisplay);
    al_destroy_timer(timer);

    al_destroy_font(ROBOTO_BOLD);
    al_destroy_font(ROBOTO_REGULAR);
    al_destroy_font(DAYCOUNTER);
    al_destroy_font(NAME);
    al_destroy_font(DISTRICT);
    al_destroy_font(CAPITOL);
    al_destroy_font(PREGAME_DISTRICT);
    al_destroy_font(PREGAME_NAME);
    al_destroy_font(PREGAME_SCORE);
    al_destroy_font(REAPING_NAME);

    al_destroy_sample(Cannon);
    al_destroy_sample(Learning_The_Skills);
    al_destroy_sample(Reaping_Day);
    al_destroy_sample(The_Horn_Of_Plenty);
    al_destroy_sample(Countdown);
    al_destroy_sample(Mockingjay);
    al_destroy_sample(Ambient);
    al_destroy_sample(Fire_End);
    al_destroy_sample(Fire_Start);
    al_destroy_sample(Fire_Loop);
    al_destroy_sample(Water_End);
    al_destroy_sample(Water_Start);
    al_destroy_sample(Water_Loop);
    al_destroy_sample(MuttSpawnSound);
    al_destroy_sample(Sponsor);
    al_destroy_sample(The_Hunger_Games);
    al_destroy_sample(Skills_Reviews);
    //al_destroy_sample(SoundDeath[Gender][100]);
    al_destroy_bitmap(The_Reaping);
    al_destroy_bitmap(Capitol_Seal);
    al_destroy_bitmap(District_Seal);
    al_destroy_bitmap(Training_Center);
    al_destroy_bitmap(Logo1);
    al_destroy_bitmap(Logo2);
    al_destroy_bitmap(icon);
    //al_destroy_bitmap(Head[NumberOfPlayers]);
    //al_destroy_bitmap(Body[NumberOfPlayers]);
}
