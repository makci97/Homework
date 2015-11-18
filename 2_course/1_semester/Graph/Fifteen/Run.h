#pragma once
#include <iostream>
#include <vector>
#include <queue>

/*
0 - Тарьян

1 - Косарайю

2 - Дейкстра

3 - Форд Беллман

4 - Флойд - Уоршал

5 - А звезда и Пятнашки

6 - Фибоначчиева куча, Дейкстра с Фибоначчиевой Кучей

7 Класс DSU. На его основе алгоритм Крускала. Граф задается как в предыдущих задачах. Вывести – одно число вес MST.

8 MST Прима, входные и выходные данные как в 7.

9 Потоки. Максимальный поток из 0 в N – 1 вершину. Описание ребер сети: М строк в каждой по четыре числа: Из Куда УдельнаяСтоимость ПропускнаяСпособность. Вывести: Два числа в одной строке: МаксимальныйПоток Стоимость.

10 Дерево Фенвика можно потестить здесь: http://acm.timus.ru/problem.aspx?space=1&num=1470

11 - Дейкстра на биномиальной куче

12 Декартово Дерево здесь: http://acm.timus.ru/problem.aspx?space=1&num=1521

13 Дерево отрезков: http://acm.timus.ru/problem.aspx?space=1&num=1717

14 LCA: http://www.e-olimp.com/problems/4812
*/

template<typename T, typename W>
int Run(std::istream &in)
{
    int flag;
    in >> flag;

    if(flag == 5)
    {
        std::vector<int> start_position;
        int j;
        for(int i = 0; i < 16; ++i)
        {
            in >> j;
            start_position.push_back(j);
        }

        Fifteen<std::priority_queue>(start_position);

        return 0;
    }
/*
    int amount_edges, amount_vertices;
    in >> amount_vertices >> amount_edges;

    Graph<T, W> graph;
    graph.ReadGraph(amount_edges, in);

    T vertex;

    if(flag == 0 || flag == 1)
        SCC(graph, flag, amount_edges, amount_vertices);
    else if(flag == 2 || flag == 3 || flag == 6 || flag == 11)
    {
        in >> vertex;
    }
    switch (flag) {
    case 2:
        Dijkstra<T, W, std::priority_queue> (graph, vertex, amount_vertices);
        break;
    case 3:
        BellmanFord<T, W> (graph, vertex, amount_vertices);
        break;
    case 4:
        FloydWarshall<T, W> (graph, amount_vertices);
        break;
    case 6:

        break;
    case 7:

        break;
    case 8:

        break;
    case 9:

        break;
    case 10:

        break;
    case 11:

        break;
    case 12:

        break;
    case 13:

        break;
    case 14:

        break;
    default:
        break;
    }
*/
    return 0;
}
/*
template<typename T, typename W>
void SCC(Graph<T, W>& graph, int flag, int amount_edges, int amount_vertices)
{
    std::vector<std::list<T> > SCC;
    if(flag == 0)
        Tarian(graph, SCC);

    else if(flag == 1)
        Kosaraju(graph, SCC);

    std::sort(SCC.begin(), SCC.end(), [&](std::list<T> first, std::list<T> second)
    {
        if(first.size() < second.size() )
            return true;
        else if(first.size() > second.size() )
            return false;
        else
        {
            return (first.front() < second.front() );
        }
    });

    std::cout << SCC.size() << std::endl;

    for(auto it_SCC = SCC.begin(), end_SCC = SCC.end();
        it_SCC != end_SCC;
        ++it_SCC)
    {
        for(auto it = it_SCC -> begin(), end = it_SCC -> end();
            it != end;
            ++it)
        {
            std::cout << *it << "\t";
        }
        std::cout << "\n";
    }

}

/*
    std::ifstream is("test_graph.txt");
    int flag, amount_edges, amount_vertices;
    is >> flag;
    is >> amount_vertices >> amount_edges;

    Graph<int, int> graph;
    ReadGraph<int, int>(graph, amount_edges, is);
*/
