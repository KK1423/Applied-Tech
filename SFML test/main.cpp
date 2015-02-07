#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
//#include <Mouse.hpp>
#define Sphere = 1;
#define Box = 2 ;
#define Plane = 3;
#define Triangle = 4;
#define pi = 3.14159;
int globalcache = 0;
bool inShapeFlag = false;
struct cameradata
{
    bool cameracalculated = false;
    float xangledeg;
    float yangledeg;

};
float rad(float degr)
{
    degr = (degr*3.14159/180.0);
    return degr;
}
float deg(float radi)
{
    return radi*(180.0/3.14159);
}
//class ray;
class d3Vector
{

public:
    float x,y,z;
    d3Vector(float X = 0,float Y = 0,float Z = 0):x(X), y(Y), z(Z) {}
    d3Vector normalize()
    {
        float d =(float)sqrtf(x*x+y*y+z*z);
        return d3Vector(x/d,y/d,z/d);
    };
    d3Vector scalarmultiply (float b)
    {
        return d3Vector((this->x*b),(this->y*b),(this->z*b));
    }
    d3Vector individualmultiply (d3Vector coefficients)
    {
        d3Vector result;
        result = d3Vector((this->x)*coefficients.x,(this->y)*coefficients.y,(this->z)*coefficients.z);
        return result;
    }
    float dot(d3Vector b)
    {
        return ((this->x*b.x)+(this->y*b.y)+(this->z*b.z));
    }
    float magnitude()
    {
        return sqrtf(pow(this->x,2)+pow(this->y,2)+pow(this->z,2));
    }
    float project(d3Vector aO,d3Vector aV)
    {
        d3Vector b=this->subtract(aO);
        d3Vector c=aO;
        aV=aV.normalize();
        return (b.dot(aV));
    }
    d3Vector add(d3Vector a)
    {
        return d3Vector((this->x)+a.x,(this->y)+a.y,(this->z)+a.z);
    }
    d3Vector subtract(d3Vector a)
    {
        return d3Vector((x)-a.x,(y)-a.y,(z)-a.z);
    }
    d3Vector rotatearoundorgin(float zenith,float attitude)
    {
        d3Vector out(0,0,0);
        d3Vector in = *this;
        float cosz = cosf(rad(zenith));
        float sinz = sinf(rad(zenith));
        float cosy = cosf(rad(attitude));
        float siny = sinf(rad(attitude));
        out = d3Vector((cosy*cosz)*in.x-(sinz)*in.y-(siny*cosz)*in.z,(cosy*sinz)*in.x+(cosz)*in.y-(siny*cosz)*in.z,(siny)*in.x+(cosy)*in.z);
        return out;
    }
};
struct objectInfo
{
    int type=0;
    d3Vector color = d3Vector(0,255,0);
    d3Vector cize = d3Vector(0,0,0);
    d3Vector pos = d3Vector(0,0,0);
    d3Vector tri1 = d3Vector(0,0,0);
    d3Vector tri2 = d3Vector(0,0,0);
    d3Vector tri3 = d3Vector(0,0,0);
    d3Vector properties = d3Vector(0.5,0,0);
    bool isLight = 0;
    float lightStrength;
};
struct hit
{
    d3Vector coord;
    d3Vector normal;
    d3Vector property;
    objectInfo hitobject;
    float t = 100.0;
    bool inter;
};
class ray
{
public:
    d3Vector O,V;
    ray(d3Vector p,d3Vector w) : O(p),V(w)
    {
        V.normalize();
    };
    void scalearound (ray m)
    {
        O = O.subtract(m.O);
        V = V.subtract(m.O);
        O = d3Vector(O.x*m.V.x,O.y*m.V.y,O.z*m.V.z);
        V = d3Vector(V.x*m.V.x,V.y*m.V.y,V.z*m.V.z);
        O = O.add(m.O);
        V = V.add(m.O);
    }
};
class d3object
{
    /**< 1-Sphere 2-Box 3-Plane 4-Triangle */
    /**< (diffuse/glossy,alpha,ior) */
public:
    objectInfo info;
    d3object(int typ,d3Vector colort,d3Vector cizet,d3Vector post,d3Vector tri1t,d3Vector tri2t,d3Vector tri3t,bool isLightt,float lightStrengtht)
    {
        info.color = colort;
        info.type = typ;
        info.cize=cizet;
        info.pos=post;
        info.tri1=tri1t;
        info.tri2=tri2t;
        info.tri3=tri3t;
        info.isLight = isLightt;
        info.lightStrength = lightStrengtht;
    }
    hit intersect (ray cameraray);
};


hit d3object::intersect (ray cameraray)
{
    switch (this->info.type)
    {
    case 1:
    {
        hit objecthit;
        ray cacheray = cameraray;

        cameraray.V = cameraray.V.normalize();
        //cameraray.scalearound(ray(info.pos,info.cize));
        float t = info.pos.project(cameraray.O,cameraray.V);
        float distance =((((cameraray.V.normalize()).scalarmultiply(t)).add(cameraray.O)).subtract(info.pos)).magnitude();
        //std::cout << distance << std::endl;
        if (distance <=1&& t>0)
        {
            float delta = sqrtf((1-pow(distance,2.0)));
            cameraray.V = cameraray.V.normalize();
            cameraray.V = cameraray.V.scalarmultiply(t-delta);
            objecthit.normal = cameraray.V.subtract(info.pos);
            //cameraray.scalearound(ray(info.pos,d3Vector(1.0/info.cize.x,1.0/info.cize.y,1.0/info.cize.z)));
            objecthit.coord=cameraray.V.add(cameraray.O);
            ray normal(info.pos,objecthit.coord.subtract(info.pos));
            objecthit.inter = true;
            objecthit.t = t-delta;
            objecthit.hitobject = info;
            objecthit.normal = objecthit.coord.subtract(info.pos);
            return objecthit;
        }
        else
        {
            objecthit.inter = false;
            return objecthit;
        }
    }
    break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;

    }
}
cameradata data;
sf::RenderWindow window(sf::VideoMode(1000, 1000), "Raytracer");
ray getCameraRay (ray camera, int x, int y, int height, int width, float jitter, float flength, float fdistance)
{

    float xfraction = ((float)x)/((float)width);
    float yfraction = ((float)y)/((float)height);
    xfraction-=0.5;
    yfraction-=0.5;
    d3Vector finaldirection(flength,xfraction,yfraction);
    finaldirection = finaldirection.normalize();
    d3Vector cache = finaldirection.scalarmultiply(fdistance);
    srand(43*x*y*globalcache*rand()%243);
    finaldirection = finaldirection.normalize();
    d3Vector aimpoint = finaldirection.scalarmultiply(fdistance);
    d3Vector startdiff((rand()%1000)*(rand()&1 ? 1:-1),(rand()%1000)*(rand()&1 ? 1:-1),(rand()%1000)*(rand()&1 ? 1:-1));
    startdiff = startdiff.normalize().scalarmultiply(0);
    finaldirection = aimpoint.subtract(startdiff);
    return ray(d3Vector(-10,0,2).add(startdiff),finaldirection);

}
class shapes
{
public:
    d3object shapez[5] =
    {
        d3object(1,d3Vector(55,15,255),d3Vector(1,1,1),d3Vector(-2.5,-6,2),d3Vector(0,0,0),d3Vector(0,0,0),d3Vector(0,0,0),true,15),
        d3object(1,d3Vector(55,55,255),d3Vector(1,1,1),d3Vector(-3,-3,4),d3Vector(0,0,0),d3Vector(0,0,0),d3Vector(0,0,0),false,3),
        d3object(1,d3Vector(255,45,255),d3Vector(1,1,1),d3Vector(3.6,1.8,1.3),d3Vector(0,0,0),d3Vector(0,0,0),d3Vector(0,0,0),false,0),
        d3object(1,d3Vector(10,255,255),d3Vector(1,1,1),d3Vector(-7,3,1),d3Vector(0,0,0),d3Vector(0,0,0),d3Vector(0,0,0),false,0),
        d3object(1,d3Vector(30,255,55),d3Vector(1,2,1),d3Vector(3.590,-0.5,1.3),d3Vector(0,0,0),d3Vector(0,0,0),d3Vector(0,0,0),true,20),
    };
};
shapes shaz;
d3Vector get_diffuse_color(int depth,hit objecthit,ray cameraray);
d3Vector get_glossy_color(int depth,hit objecthit,ray cameraray);
d3Vector get_transmit_color(int depth,hit objecthit,ray cameraray);
d3Vector trace(ray traced_ray,int depth,shapes shaz)
{

    //std::cout << traced_ray.V.x << " " << traced_ray.V.y << " " << traced_ray.V.z;
    if (depth==0)
    {

        //std::cout << "fin";
        return d3Vector(127,127,127);
    }
    // std::cout << "made it2";
    float empty[5];
    float lowest = 10000;
    int lowestindex;
    hit objecthit;
    hit hittrash[5];
    //std::cout << "made it3";
    for (int a = 0; a<5; a++)
    {
        hittrash[a] = shaz.shapez[a].intersect(traced_ray);
        //if(hittrash[a].inter){std::cout << "fin";}
    };
    for (int a = 0; a<5; a++)
    {
        empty[a] = hittrash[a].t;
    };
    for (int a = 0; a<5; a++)
    {
        if (empty[a]<lowest)
        {
            lowest = empty[a];
            lowestindex = a;
        }
    };
    //objecthit = hittrash[lowestindex];
    //std::cout<<lowestindex;
    // std::cout << "made it4";
    //std::cout << "made it5";
    objecthit = hittrash[lowestindex];
    if(!objecthit.inter)
    {
        srand(rand()%4326);
        lowest = 100;
        return d3Vector(lowest,lowest,lowest);
    };
    if(objecthit.hitobject.isLight==true)
    {
        return objecthit.hitobject.color.scalarmultiply(objecthit.hitobject.lightStrength);
    }
    objecthit.normal = objecthit.normal.normalize();
    d3Vector diffuse_color;
    int n = 0;
    while (n<30)
    {
        diffuse_color = diffuse_color.add(get_diffuse_color(depth,objecthit,traced_ray));
        n++;
    }
    diffuse_color = diffuse_color.scalarmultiply(1.0/n);
    n=0;
    d3Vector glossy_color = get_glossy_color(depth,objecthit,traced_ray);
    //glossy_color = glossy_color.scalarmultiply(100.0/((traced_ray.O.subtract(objecthit.coord).magnitude())*(traced_ray.O.subtract(objecthit.coord).magnitude())));
    //d3Vector transmit_color = get_transmit_color(depth,objecthit,traced_ray);
    glossy_color = (glossy_color.scalarmultiply(objecthit.hitobject.properties.x)).add(diffuse_color.scalarmultiply(1 - objecthit.hitobject.properties.x));
    //glossy_color = (glossy_color.scalarmultiply(objecthit.hitobject.properties.y)).add(transmit_color.scalarmultiply(1 - objecthit.hitobject.properties.y));
    for (int a = 0; a<5; a++)
    {
        hittrash[a] = shaz.shapez[a].intersect(traced_ray);
        //if(hittrash[a].inter){std::cout << "fin";}
    };
    return glossy_color;
}
d3Vector get_glossy_color(int depth,hit objecthit,ray cameraray)
{
    cameraray.V = cameraray.V.scalarmultiply(objecthit.t);
    ray nextray(d3Vector(0,0,0),d3Vector(0,0,0));
    d3Vector projectablecameraV = cameraray.V.scalarmultiply(-1.0);
    objecthit.normal = objecthit.normal.normalize();
    nextray.O = objecthit.normal.scalarmultiply(objecthit.normal.dot(projectablecameraV));
    nextray.V = nextray.O.subtract(projectablecameraV);
    nextray.O = nextray.O.add(nextray.V);
    nextray = ray(objecthit.coord,nextray.O);
    nextray.O = nextray.O.add(objecthit.normal.scalarmultiply(0.01));
    nextray.V = nextray.V.normalize();
    srand(rand()%663);
    nextray.V = nextray.V.add((d3Vector((rand()%20)/200.0,(rand()%20)/200.0,(rand()%20)/200.0)));
    //std::cout << nextray.V.x;
    return objecthit.hitobject.color.individualmultiply(trace(nextray,depth - 1,shaz).scalarmultiply(1.0/255.0));
}
d3Vector get_diffuse_color(int depth,hit objecthit,ray cameraray)
{
    srand(rand()+((globalcache+3)*(depth+4)));
    d3Vector randvec((rand()%10000)*(rand()&1 ? 1:-1),(rand()%10000)*(rand()&1 ? 1:-1),(rand()%10000)*(rand()&1 ? 1:-1));
    //std::cout << "made it8"<<depth;
    while(randvec.dot(objecthit.normal)<0)
    {
        randvec = d3Vector((rand()%100)*(rand()&1 ? 1:-1),(rand()%100)*(rand()&1 ? 1:-1),(rand()%100)*(rand()&1 ? 1:-1)).normalize();
    }
    randvec = randvec.normalize();
    return objecthit.hitobject.color.individualmultiply(trace(ray(objecthit.coord,randvec),depth - 1,shaz).scalarmultiply(randvec.dot(objecthit.normal)/255.0));

}
sf::Image shape;
sf::Color computePixel (int x, int y,int seed)
{
    srand(seed);
    sf::Color old = shape.getPixel(x,y);
    d3Vector oldv(old.r,old.g,old.b);
    d3Vector color = trace(getCameraRay(ray(d3Vector(7.481,-6.507,5.343),d3Vector(-1.214,1.132,-0.825)),x,y,1000,1000,0.1,0.5,15.53),2,shaz);
    color = d3Vector(color.x>254 ? 255:color.x,color.y>254 ? 255:color.y,color.z>254 ? 255:color.z);
    return sf::Color(color.x,color.y,color.z);
}

int main()
{
    srand(5);
    //sf::Image shape;
    int n = 1;
    sf::Texture texture;
    texture.create(1000,1000);
    sf::Sprite sprite(texture);
    shape.create(1000,1000,sf::Color::Black);
    srand(rand());
    while (window.isOpen())
    {
        int x=0,y=0;
        sf::Event event;
        while(x<999)
        {
            srand(rand()%1000);
            while(y<999)
            {
                shape.setPixel(x,y,computePixel(x,y,rand()));
                y++;
            }
            y = 0;
            x++;
            if (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            if((x%300)==1)
            {
                texture.update(shape);
                sprite.setTexture(texture,false);
                window.draw(sprite);
                window.display();
            }
            if(!window.isOpen())
            {
                break;
            };
        }
        x = 0;
        globalcache++;
        shaz = shapes();
        texture.update(shape);
        sprite.setTexture(texture,false);
        window.draw(sprite);
        window.display();

        /* if(1 == 1)
         {
             shape.create(1000,1000,sf::Color::Black);
         }*/
        //sf::sleep(sf::milliseconds(10));
    }

    return 0;
}


