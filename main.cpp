#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#define eps 1.e-4
using namespace std;

typedef struct Point
{
    float x, y;
};

Point pLocate;///punctul pe care vreau sa il localizez

int test = 0;
class MooPolygon
{
    public: void afisare()
{
    for(int i=0;i<points.size();i++)
    {
        cout<<points[i].x<<" "<<points[i].y<<endl;

    }
    cout<<endl;
};

private:
    vector<Point> points;

    int isVertexEar(int n, const vector<Point> &p)
    {
        return (isVertexInsideNewPoly(n,p) && !isEdgeIntersect(n, p));
    }

    int isEdgeIntersect(int n, const vector<Point> &p)
    {
        Point v = p[n];
        vector<Point> a;

        for (size_t i = 0; i < p.size(); i++)
            if (i != n)
                a.push_back(p[i]);

        int c = 0, cnt = a.size(), prev = (cnt + (n - 1)) % cnt, next = n % cnt;
        Point v1 = a[prev], v2 = a[next];

        for (size_t i = 0, j = cnt - 1; i < cnt; j = i++)
        {
            if (prev == i || prev == j || next == i || next == j)
                continue;

            Point v4 = a[j], v3 = a[i];

            float denominator = ((v4.y - v3.y) * (v2.x - v1.x)) - ((v4.x - v3.x) * (v2.y - v1.y));

            if (!denominator)
                continue;

            float ua = (((v4.x - v3.x) * (v1.y - v3.y)) - ((v4.y - v3.y) * (v1.x - v3.x))) / denominator;
            float ub = (((v2.x - v1.x) * (v1.y - v3.y)) - ((v2.y - v1.y) * (v1.x - v3.x))) / denominator;

            //float x = v1.x + (ua * (v2.x - v1.x)), y = v1.y + (ua * (v2.y - v1.y));

            if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
            {
                c = 1;
                break;
            }
        }
        return c;
    }

    int isVertexInsideNewPoly(int n, const vector<Point> &p)///foloseste ray casting algoritm
    {
        Point v = p[n];
        vector<Point> a;
        ///pun in vectorul a toate elementele inafara de cel de poz n
        for (size_t i = 0; i < p.size(); i++)
            if (i != n)
                a.push_back(p[i]);
        int c = 1;

        for (size_t i = 0, j = a.size() - 1; i < a.size(); j = i++)
        {  ///iau toate laturile din vectorul asi verific daca se intersecteaza cu ele
            ///daca se intersecteaza de un numar impar de ori punctul este in interior
            ///daca e numar par punctul se intersecteaza de un numar par de ori
            if ((((a[i].y <= v.y) && (v.y < a[j].y)) ||
                    ((a[j].y <= v.y) && (v.y < a[i].y)))&&
                    (v.x > (a[j].x - a[i].x) *(v.y - a[i].y)/ (a[j].y - a[i].y) + a[i].x))///???
                c = !c;
           //cout<<((a[j].x - a[i].x) *(v.y - a[i].y)/ (a[j].y - a[i].y) + a[i].x)<<" calcul"<<endl;

        }
        //cout<"inside";
        return c;
        ///0-daca e in interior
        ///in afara
    }

    ///distanta dintre doua puncte
    float dist(Point a, Point b)
    {
        return sqrt(  ((a.x - b.x) * (a.x - b.x)) + (((a.y - b.y) * (a.y - b.y)))  );
    }

    ///testul de orientare
    int orientare(Point a,Point b,Point c)//p,q,r
    {
        double val = (b.y-a.y)*(c.x-b.x)-(b.x-a.x)*(c.y-b.y);
        if(val==0) return 0; //coliniaritate
        return (val>0)? 1: 2; //1 pentru dreapta , 2 pentru stanga
    }


public:
    void push(const Point &p)
    {

        points.push_back(p);///daca nu e nimic in vector adaug p
    }
/// pentru a afisa triangularile dupa
    void display()
    {
        for(vector <Point>::iterator i= points.begin(); i<points.end(); i++)
            {cout<<" Triunghiul format de punctele: ("<<i->x<<","<<i->y<<") ";
             i++;
             cout<<"("<<i->x<<","<<i->y<<") ";
             i++;
             cout<<"("<<i->x<<","<<i->y<<")"<<endl;}
    }

    void pop()
    {
        if (points.size() > 0)
            points.pop_back();
    }

    void clear()
    {
        points.clear();///sterge toate elem din vector
    }

    Point v(int index)
    {
        return points[index];
    }

    size_t size()
    {
        return points.size();
    }

    void triangulate()
    {
        vector<Point> a;

        for (size_t i = 0; i < points.size(); i++)
        {
            a.push_back(points[i]);
        }
        ///am copiat in a vectorul points
        points.clear();///sterg tot din vector

        for (size_t t = a.size() - 1, i = 0, j = 1; i < a.size(); t = i++, j = (i + 1) % a.size())
        {
            if (a.size() == 3)
            {
                points.push_back(a[0]);
                points.push_back(a[1]);
                points.push_back(a[2]);
                //printf(" (%.1f ,%.1f) , (%.1f , %.1f) , (%.1f , %.1f)\n",a[0].x,a[0].y,a[1].x,a[1].y,a[2].x,a[2].y);
                if(test!=1)
                {   ///verific daca punctul este pe laturile vreunui triunghi, cu testul de orientare
                    if(orientare(a[0],a[1],pLocate)== 0 || orientare(a[1],a[2],pLocate) == 0 || orientare (a[2],a[0],pLocate) == 0 )
                    {
                        printf("Punctul dat se afla in interiorul triunghiului format de punctele: (%.1f %.1f), (%.1f %.1f) ,(%.1f %.1f)\n",a[0].x,a[0].y,a[1].x,a[1].y,a[2].x,a[2].y);
                        test = 1;///marchez ca am gasit
                    }
                    ///verific cu testul de orientare daca e in interiorul unui triunghi
                    else if(orientare(a[0],pLocate,a[1]) == orientare(a[1],pLocate,a[2]) &&
                            orientare(a[0],pLocate,a[1]) == orientare (a[2],pLocate,a[0] ) )
                    {
                        printf("Punctul dat se afla in interiorul triunghiului format de punctele: (%.1f %.1f), (%.1f %.1f) ,(%.1f %.1f)\n",a[0].x,a[0].y,a[1].x,a[1].y,a[2].x,a[2].y);
                        test = 1;///marchez ca am gasit
                    }
                }
                break;
            }

            if (isVertexEar(i, a))
            {
                points.push_back(a[t]);
                points.push_back(a[i]);
                points.push_back(a[j]);
                //     printf(" (%.1f ,%.1f) , (%.1f , %.1f) , (%.1f , %.1f)\n",a[t].x,a[t].y,a[i].x,a[i].y,a[j].x,a[j].y);
                if(test!=1)
                {
                     ///verific daca punctul pLocate este pe laturile vreunui triunghi
                     ///test de orientare
                    if(orientare(a[t],a[i],pLocate)== 0 || orientare(a[i],a[j],pLocate) == 0 || orientare (a[j],a[t],pLocate) == 0 )
                    {
                        printf("Punctul dat se afla in interiorul triunghiului format de punctele: (%.1f, %.1f), (%.1f, %.1f ), (%.1f, %.1f)\n",a[t].x,a[t].y,a[i].x,a[i].y,a[j].x,a[j].y);
                        test = 1;
                    }
                     ///verific daca este in interiorul vreunui triunghi
                     ///teste de orientare
                    else if(orientare(a[t],pLocate,a[i])== orientare(a[i],pLocate,a[j]) &&
                            orientare(a[t],pLocate,a[i]) == orientare (a[j],pLocate,a[t]))
                    {
                        printf("Punctul dat se afla in interiorul triunghiului format de punctele: (%.1f,%.1f), (%.1f, %.1f), (%.1f, %.1f)\n",a[t].x,a[t].y,a[i].x,a[i].y,a[j].x,a[j].y);
                        test = 1;
                    }
                }

                a.erase(a.begin() + i, a.begin() + i + 1);///sterg elementul de pe poz i din vector

                t = a.size() - 1;
                i = 0;
                j = 1;
            }
        }
    }
    void algoritm()
    {
        int i, j ;
        int n = points.size();
        for( i = 0,j = (i+1)%n; i<n-1; i++,j=i+1)
        {   ///verific inca de la citire daca cumva punctul se afla pe o latura a poligonului
            if(orientare(points[i],pLocate,points[j])==0)
            {
                printf("Punctul dat se afla pe latura formata de punctele: ( %.1f,%.1f) si (%.1f,%.1f) \n",points[i].x,points[i].y,points[j].x,points[j].y);
                test = 1;
                break;
            }
        }
        triangulate();
        return;
    }
};
int main()
{
    int n;
    fstream f("date.in");
    f>>pLocate.x>>pLocate.y;
    f>>n;

    Point aux;
    MooPolygon poly;

    for(int i = 0 ; i<n; i++)
    {
        f>>aux.x>>aux.y;
        poly.push(aux);
    }


    poly.algoritm();
    ///nu am gasit punctul in niciun triunghi si nici pe laturile vreunuia
    if(test==0)
        cout<<"In exterior";

    printf("Triangulare\n");
    poly.display();
    f.close();

    return 0;

}

//varfuri concave -> varfuri cu unghiul mai mare de 180 grade
