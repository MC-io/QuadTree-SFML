#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1000
#define HEIGHT 1000

#include "SFML/Graphics.hpp"

class QuadTree;

class Point
{
private:
    int x;
    int y;
    friend class Node;
    friend class QuadTree;
public:
    Point()
    {
        this->x = this-> y = 0;
    }
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int get_x() const
    {
        return this->x;
    }
    int get_y() const
    {
        return this->y;
    }
};

class Node
{
private:
    Point point;
    Point right_up;
    Point left_down;
    Node * north_east;
    Node * north_west;
    Node * south_west;
    Node * south_east;
    Node * parent;
    bool leaf;
    friend class QuadTree;
public:
    Node()
    {
        this->point = Point();
        this->north_east = nullptr;
        this->north_west = nullptr;
        this->south_west = nullptr;
        this->south_east = nullptr;
        this->parent = nullptr;
        this->leaf = true;
    }
    Node(int x, int y)
    {
        this->point = Point(x,y);
        this->north_east = nullptr;
        this->north_west = nullptr;
        this->south_west = nullptr;
        this->south_east = nullptr;
        this->parent = nullptr;
        this->leaf = true;

    }
    Point get_right_up()
    {
        return this->right_up;
    }
    
    Point get_left_down()
    {
        return this->left_down;
    }
    Node * get_parent()
    {
        return this->parent;
    }
    Point get_point()
    {
        return this->point;
    }
    bool is_leaf()
    {
        return this->leaf;
    }
};
class QuadTree
{
private:
    Node * root;
public:
    QuadTree()
    {
        this->root = nullptr;
    }
    Node * insert(int x, int y)
    {
        if (!this->root)
        {
            this->root = new Node(x,y);
            this->root->parent = nullptr;
            this->root->right_up = Point(WIDTH,HEIGHT);
            this->root->left_down = Point(0,0);
            return this->root;
        }
        Node  * tmp = this->root;
        Node  * tmp2 = tmp;
        while(tmp)
        {
            tmp2 = tmp;
            if (tmp->point.x > x && tmp->point.y > y)
            {
                tmp = tmp->south_west;
            }
            
            else if (tmp->point.x <= x && tmp->point.y <= y)
            {
                tmp = tmp->north_east;
            }
            
            else if (tmp->point.x < x && tmp->point.y > y)
            {
                tmp = tmp->south_east;
            }
             
            else if (tmp->point.x > x && tmp->point.y < y)
            {
                tmp = tmp->north_west;
            }
        }
        Node * new_node = new Node(x,y);
        if (tmp2->leaf == true) tmp2->leaf = false;
        if (tmp2->point.x > x && tmp2->point.y > y)
        {
            tmp2->south_west = new_node;
            tmp2->south_west->right_up = tmp2->point;
            tmp2->south_west->left_down = tmp2->left_down;

        }
        
        else if (tmp2->point.x < x && tmp2->point.y < y)
        {
            tmp2->north_east = new_node;
            tmp2->north_east->right_up = tmp2->right_up;
            tmp2->north_east->left_down = tmp2->point;

        }
        
        else if (tmp2->point.x < x && tmp2->point.y > y)
        {
            tmp2->south_east = new_node;
            tmp2->south_east->right_up = Point(tmp2->right_up.x,tmp2->point.y);
            tmp2->south_east->left_down = Point(tmp2->point.x,tmp2->left_down.y);

        }
         
        else if (tmp2->point.x > x && tmp2->point.y < y)
        {
            tmp2->north_west = new_node;
            tmp2->north_west->right_up = Point(tmp2->point.x,tmp2->right_up.y);
            tmp2->north_west->left_down = Point(tmp2->left_down.x,tmp2->point.y);
        }
        new_node->parent = tmp2;
        return new_node;
    }


    void query_range(Node * node, Point a, Point b, std::vector<Point> & points)
    {
        // If the point is inside
        if (node->get_point().get_x() >= a.get_x() && node->get_point().get_y() >= a.get_y() &&
        node->get_point().get_x() <= b.get_x() && node->get_point().get_y() <= b.get_y())
        {
            points.push_back(node->get_point());
        }

        Point c(a.get_x(),b.get_y());
        Point d(b.get_x(),a.get_y());
        if (b.get_x() >= node->get_point().get_x() && b.get_y() >= node->get_point().get_y() && node->north_east)
        {
            this->query_range(node->north_east, a, b, points);
        }
        if (d.get_x() >= node->get_point().get_x() && d.get_y() < node->get_point().get_y() && node->south_east)
        {
            this->query_range(node->south_east, a, b, points);
        }
        if (c.get_x() < node->get_point().get_x() && b.get_y() >= node->get_point().get_y() && node->north_west)
        {
            this->query_range(node->north_west, a, b, points);
        }
        if (a.get_x() < node->get_point().get_x() && a.get_y() < node->get_point().get_y() && node->south_west)
        {
             this->query_range(node->south_west, a, b, points);
        }

        /*
        // northeast
        if (b.get_x() >= node->get_point().get_x() && b.get_y() >= node->get_point().get_y())
        {
            if (a.get_x() >= node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, a, b, points);
            else if (a.get_x() >= node->get_point().get_x() && a.get_y() < node->get_point().get_y())
                this->query_range(node->north_east, Point(a.get_x(), node->get_point().get_y()), b, points);
            else if (a.get_x() < node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, Point(node->point.get_x(), a.get_y()), b, points);
            else 
                this->query_range(node->north_east, Point(node->point.get_x(), node->point.get_y()), b, points);
        }
        //southeast
        if (d.get_x() >= node->get_point().get_x() && d.get_y() < node->get_point().get_y())
        {
            if (c.get_x() >= node->get_point().get_x() && c.get_y() < node->get_point().get_y())
                this->query_range(node->north_east, a, b, points);
            else if (c.get_x() < node->get_point().get_x() && c.get_y() < node->get_point().get_y())
                this->query_range(node->north_east, Point(c.get_x(), node->get_point().get_y()), b, points);
            else if (c.get_x() < node->get_point().get_x() && c.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, Point(node->point.get_x(), a.get_y()), b, points);
            else 
                this->query_range(node->north_east, Point(node->point.get_x(), node->point.get_y()), b, points);
        }
        // northwest
        if (c.get_x() < node->get_point().get_x() && b.get_y() >= node->get_point().get_y())
        {
            if (a.get_x() >= node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, a, b, points);
            else if (a.get_x() >= node->get_point().get_x() && a.get_y() < node->get_point().get_y())
                this->query_range(node->north_east, Point(a.get_x(), node->get_point().get_y()), b, points);
            else if (a.get_x() < node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, Point(node->point.get_x(), a.get_y()), b, points);
            else 
                this->query_range(node->north_east, Point(node->point.get_x(), node->point.get_y()), b, points);
        }
        // southwest
        if (a.get_x() < node->get_point().get_x() && a.get_y() < node->get_point().get_y())
        {
            if (a.get_x() >= node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, a, b, points);
            else if (a.get_x() >= node->get_point().get_x() && a.get_y() < node->get_point().get_y())
                this->query_range(node->north_east, Point(a.get_x(), node->get_point().get_y()), b, points);
            else if (a.get_x() < node->get_point().get_x() && a.get_y() >= node->get_point().get_y())
                this->query_range(node->north_east, Point(node->point.get_x(), a.get_y()), b, points);
            else 
                this->query_range(node->north_east, Point(node->point.get_x(), node->point.get_y()), b, points);
        }
        */
    }
    
    
    std::vector<Point> query_range_search(Point a, Point b)
    {
        std::vector<Point> points;
        this->query_range(this->root,a,b,points);
        return points;
    }

    void print_pre_order(Node * node)
    {
        if (!node) return;
        std::cout << node->point.x << ' ' << node->point.y << '\n';
        print_pre_order(node->north_east);
        print_pre_order(node->north_west);
        print_pre_order(node->south_west);
        print_pre_order(node->south_east);
    }
    void print_pre_order()
    {
        print_pre_order(this->root);
    }
};

int main() 
{
    srand(time(0));
    QuadTree my_tree;
    int T = 50;
    std::vector<sf::VertexArray> lines;
    std::vector<sf::CircleShape> points;
    for (int i = 0; i < T; i++)
    {
        int j = 0;
        int x = rand() % 1000 + 1, y = rand() % 1000;
        Node * mi_nodo = my_tree.insert(x,y);

        sf::CircleShape point;
        point.setPosition(mi_nodo->get_point().get_x() - 5, mi_nodo->get_point().get_y() - 5);
        point.setRadius(5);
        point.setFillColor(sf::Color::Red);
        points.push_back(point);

        Node * nodo = mi_nodo->get_parent();
        if (!nodo) continue;
        int px = nodo->get_point().get_x();
        int py = nodo->get_point().get_y();

        int x1 = nodo->get_right_up().get_x();
        int y1 = nodo->get_right_up().get_y();
        int x2 = nodo->get_left_down().get_x();
        int y2 = nodo->get_left_down().get_y();

        sf::VertexArray line1(sf::LineStrip, 2);
        line1[0].position = sf::Vector2f(px, y2);
        line1[1].position = sf::Vector2f(px, y1);
        line1[0].color = sf::Color::Black;
        line1[1].color = sf::Color::Black;



        lines.push_back(line1);

        sf::VertexArray line2(sf::LineStrip, 2);
        line2[0].position = sf::Vector2f(x1, py);
        line2[1].position = sf::Vector2f(x2, py);

        line2[0].color = sf::Color::Black;
        line2[1].color = sf::Color::Black;

        lines.push_back(line2);
    }

    my_tree.print_pre_order();
    int ax = 100, ay = 100, bx = 800, by = 800;

    std::vector<Point> range = my_tree.query_range_search(Point(ax,ay),Point(bx,by));
    
    sf::VertexArray range_border1(sf::LineStrip, 2);
    sf::VertexArray range_border2(sf::LineStrip, 2);
    sf::VertexArray range_border3(sf::LineStrip, 2);
    sf::VertexArray range_border4(sf::LineStrip, 2);
    
    range_border1[0].position = sf::Vector2f(ax,ay);
    range_border1[1].position = sf::Vector2f(ax,by);
    range_border1[0].color = sf::Color::Green;
    range_border1[1].color = sf::Color::Green;

    range_border2[0].position = sf::Vector2f(ax,ay);
    range_border2[1].position = sf::Vector2f(bx,ay);
    range_border2[0].color = sf::Color::Green;
    range_border2[1].color = sf::Color::Green;

    range_border3[0].position = sf::Vector2f(ax,by);
    range_border3[1].position = sf::Vector2f(bx,by);
    range_border3[0].color = sf::Color::Green;
    range_border3[1].color = sf::Color::Green;

    range_border4[0].position = sf::Vector2f(bx,by);
    range_border4[1].position = sf::Vector2f(bx,ay);
    range_border4[0].color = sf::Color::Green;
    range_border4[1].color = sf::Color::Green;
    lines.push_back(range_border1);
    lines.push_back(range_border2);
    lines.push_back(range_border3);
    lines.push_back(range_border4);




    
    std::cout << "Puntos en rango:\n";

    for (const auto &a : range)
    {
        std::cout << a.get_x() << ' ' << a.get_y() << '\n';
    }

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "QuadTree Visualization");

    // Start the game loop
    while (window.isOpen()) 
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear(sf::Color::White);
        for (int i = 0; i < points.size(); i++)
            window.draw(points[i]);

        for (int i = 0; i < lines.size(); i++)
            window.draw(lines[i]);
        // Update the window


        window.display();
    }
    return EXIT_SUCCESS;

    return 0;
}

