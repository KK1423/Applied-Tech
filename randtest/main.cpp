#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;
//sf::RenderWindow window(sf::VideoMode(1000, 1000), "Raytracer");

int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 1000), "SFML works!");
    //sf::CircleShape shape(100.f);
    sf::Image image;
    image.create(512,1000,sf::Color::Black);
    sf::Texture texture;
    texture.create(512,1000);
    sf::Sprite sprite(texture);
    int x=1;
    int y=1;
    //shape.setFillColor(sf::Color::Green);
    int n[515];
    bool s[515];
    for(int i = 0; i<516; i++)
    {
        n[i] = rand()%1000;
        s[i] = rand()&1;
    }
    srand(5);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        for(int i = 0; i<10000; i++)
        {
            int m = rand()&511;
            if(n[m]>=999)s[m]=true;
            if(n[m]<=5)s[m]=false;
            n[m] = (n[m]+(s[m]?-1:1));
            int next = m + (rand()%10)*(rand()&1?1:-1);
            if(next<=0) next+=512;
            if(next>=512) next-=512;
            if((rand()%2)==0)n[m] = (n[next]+n[m])/2;
            next = m + (rand()%5)*(rand()&1?1:-1);
            if(next<=0) next+=512;
            if(next>=512) next-=512;
            if((rand()%5)==0)s[m] = s[next];
            if((rand()%1000000)==1)
            {
                int k;
                for(int j = -10;j<=10;j++)
                {
                    if(m+j<0)
                    {k = 511+(m+j);}else{k = (m+j)%511;}
                        n[k]=n[k]-(50*(s[m]?-1:1)*(abs(11-j)));

                }
            }
        }
        sf::sleep(sf::Time(sf::milliseconds(0)));
        while(x<512)
        {
            while(y<1000)
            {
                //cout<<x<<endl;
                image.setPixel(x,y,(y <(n[x]))?sf::Color(s[x]*255,0,rand()%255):sf::Color::White);
                //if(y<10)
                   // image.setPixel(x,y,s[x]?sf::Color::Red:sf::Color::Blue);
                y++;
            }
            x++;
            y = 0;
        }
        x = 1;
        y = 1;
        texture.update(image);
        sprite.setTexture(texture,false);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
