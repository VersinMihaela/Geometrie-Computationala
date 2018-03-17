
//A polygon and a point are given by their coordinates.
// Determine a triangulation of the polygon and relative
// position of that point relative to the polygon.
//If it's inside the polygon, show the triangle it's part of.

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

Point pLocate;///the point I want to locate

int test = 0;
class Polygon
{
public:
    void print()
    {
        for(int i=0; i<points.size(); i++)
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
        vector<Point> aux;

        for (size_t i = 0; i < p.size(); i++)
            if (i != n)
                aux.push_back(p[i]);

        int c = 0, cnt = aux.size(), prev = (cnt + (n - 1)) % cnt, next = n % cnt;
        Point v1 = aux[prev], v2 = aux[next];

        for (size_t i = 0, j = cnt - 1; i < cnt; j = i++)
        {
            if (prev == i || prev == j || next == i || next == j)
                continue;

            Point v4 = aux[j], v3 = aux[i];

            float denominator = ((v4.y - v3.y) * (v2.x - v1.x)) - ((v4.x - v3.x) * (v2.y - v1.y));

            if (!denominator)
                continue;

            float ua = (((v4.x - v3.x) * (v1.y - v3.y)) - ((v4.y - v3.y) * (v1.x - v3.x))) / denominator;
            float ub = (((v2.x - v1.x) * (v1.y - v3.y)) - ((v2.y - v1.y) * (v1.x - v3.x))) / denominator;

            if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
            {
                c = 1;
                break;
            }
        }
        return c;
    }

    ///this method uses ray casting algorithm
    int isVertexInsideNewPoly(int n, const vector<Point> &p)
    {
        Point v = p[n];
        vector<Point> aux;
        ///I put in the aux vector all the elements other than the n-element
        for (size_t i = 0; i < p.size(); i++)
        {
            if (i != n)
                aux.push_back(p[i]);
        }

        int c = 1;

        for (size_t i = 0, j = aux.size() - 1; i < aux.size(); j = i++)
        {
            ///I take all sides of the vector a and check if it intersects with them
            ///if it intersects an odd number of times the point is inside
            ///if it intersects an even number of times, then the point is located outside
            if ((((aux[i].y <= v.y) && (v.y < aux[j].y)) ||
                    ((aux[j].y <= v.y) && (v.y < aux[i].y)))&&
                    (v.x > (aux[j].x - aux[i].x) *(v.y - aux[i].y)/ (aux[j].y - aux[i].y) + aux[i].x))
                c = !c;

        }
        return c;
        /// will return 0-if it's inside
        ///will return 1 if it's outside
    }

    ///this function calculates the distance between two points
    float dist(Point point1, Point point2)
    {
        return sqrt(  ((point1.x - point2.x) * (point1.x - point2.x)) + (((point1.y - point2.y) * (point1.y - point2.y)))  );
    }

    ///function for the orientation test
    int orientation(Point point1,Point point2,Point point3)//p,q,r
    {
        double value = (point2.y-point1.y)*(point3.x-point2.x)-(point2.x-point1.x)*(point3.y-point2.y);
        if(value==0) return 0; //coliniaritate
        return (value>0)? 1: 2; //1 pentru dreapta , 2 pentru stanga
    }


public:
    void push(const Point &point)
    {
        points.push_back(point);
    }

    /// function made to display the triangulations
    void display()
    {
        for(vector <Point>::iterator i= points.begin(); i<points.end(); i++)
        {
            cout<<"triangle made of : ("<<i->x<<","<<i->y<<") ";
            i++;
            cout<<"("<<i->x<<","<<i->y<<") ";
            i++;
            cout<<"("<<i->x<<","<<i->y<<")"<<endl;
        }
    }

    void pop()
    {
        if (points.size() > 0)
            points.pop_back();
    }

    void clear()
    {
        ///deletes all the elements from the vector
        points.clear();
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
        vector<Point> aux;
        ///aux is an auxiliary vector. I copied in aux all the elements from the points vector

        for (size_t i = 0; i < points.size(); i++)
        {
            aux.push_back(points[i]);
        }
        ///deletes all the elements from the vector
        points.clear();

        for (size_t t = aux.size() - 1, i = 0, j = 1; i < aux.size(); t = i++, j = (i + 1) % aux.size())
        {
            if (aux.size() == 3)
            {
                ///test- the variable with which I mark whether or not I found the point
                ///test=1- means I found the point
                ///test=0- means I  have not yet found the point inside of any triangle
                points.push_back(aux[0]);
                points.push_back(aux[1]);
                points.push_back(aux[2]);
                if(test!=1)
                {
                    /// I Check if the point is on the sides of any triangle with the orientation test
                    if(orientation(aux[0],aux[1],pLocate)== 0 || orientation(aux[1],aux[2],pLocate) == 0 || orientation(aux[2],aux[0],pLocate) == 0 )
                    {
                        printf("PThe point is inside of the triangle formed by the points: (%.1f %.1f), (%.1f %.1f) ,(%.1f %.1f)\n",aux[0].x,aux[0].y,aux[1].x,aux[1].y,aux[2].x,aux[2].y);
                        test = 1;
                    }
                    ///I check with the orientation test if it's inside of a triangle
                    else if(orientation(aux[0],pLocate,aux[1]) == orientation(aux[1],pLocate,aux[2]) &&
                            orientation(aux[0],pLocate,aux[1]) == orientation(aux[2],pLocate,aux[0] ) )
                    {
                        printf("The point is inside of the triangle formed by the points: (%.1f %.1f), (%.1f %.1f) ,(%.1f %.1f)\n",aux[0].x,aux[0].y,aux[1].x,aux[1].y,aux[2].x,aux[2].y);
                        test = 1;
                    }
                }
                break;
            }

            if (isVertexEar(i, aux))
            {
                points.push_back(aux[t]);
                points.push_back(aux[i]);
                points.push_back(aux[j]);
                if(test!=1)
                {
                    ///I check if the pLocate point is on the sides of any triangle
                    /// I use the orientation test
                    if(orientation(aux[t],aux[i],pLocate)== 0 || orientation(aux[i],aux[j],pLocate) == 0 || orientation(aux[j],aux[t],pLocate) == 0 )
                    {
                        printf("The point is inside of the triangle formed by the points: (%.1f, %.1f), (%.1f, %.1f ), (%.1f, %.1f)\n",aux[t].x,aux[t].y,aux[i].x,aux[i].y,aux[j].x,aux[j].y);
                        test = 1;
                    }
                    ///I check if the pLocate point is inside of any triangle
                    ///I use the orientation test
                    else if(orientation(aux[t],pLocate,aux[i])== orientation(aux[i],pLocate,aux[j]) &&
                            orientation(aux[t],pLocate,aux[i]) == orientation(aux[j],pLocate,aux[t]))
                    {
                        printf("The point is inside of the triangle formed by the points: (%.1f,%.1f), (%.1f, %.1f), (%.1f, %.1f)\n",aux[t].x,aux[t].y,aux[i].x,aux[i].y,aux[j].x,aux[j].y);
                        test = 1;
                    }
                }
                ///I delete the element from the i position of the vector
                aux.erase(aux.begin() + i, aux.begin() + i + 1);

                t = aux.size() - 1;
                i = 0;
                j = 1;
            }
        }
    }
    void algorithm()
    {
        int i, j ;
        int vectorLenght = points.size();
        for( i = 0,j = (i+1)%vectorLenght; i<vectorLenght-1; i++,j=i+1)
        {
            ///check from reading if the point is somewhere on the side of the polygon
            if(orientation(points[i],pLocate,points[j])==0)
            {
                printf("The point is on the side of the triangle formed by the points: ( %.1f,%.1f) si (%.1f,%.1f) \n",points[i].x,points[i].y,points[j].x,points[j].y);
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
    int vectorLength;
    fstream f("date.in");
    ///the point I want to locate
    f>>pLocate.x>>pLocate.y;
    f>>vectorLength;

    Point aux;
    Polygon poly;

    for(int i = 0 ; i<vectorLength; i++)
    {
        f>>aux.x>>aux.y;
        poly.push(aux);
    }


    poly.algorithm();
    ///for the case where I did not find the point in any triangle or the sides of any one
    if(test==0)
        cout<<"Phe point is on the outside of the polygon";

    printf("The triangulation\n");
    poly.display();
    f.close();

    return 0;

}

