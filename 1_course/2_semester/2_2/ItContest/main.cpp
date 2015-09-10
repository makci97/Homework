#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <cassert>
#include <algorithm>

class Worker
{
public:

    void AddTask(int balls)
    {
        _tasks.push_back(balls);
        _solutions_qty_per_task.push_back({_solutions_qty_per_task.size() + 1, 0});
    }

    void StudentDidSolveTask(const std::string &name, int task_number)
    {
        if (_students.find(name) == _students.end())
        {
            std::pair<std::string, std::set<int>> pair;
            pair.first = name;
            pair.second.insert(task_number);
            _students.insert(pair);

            _students_task_qty_top.push_back(name);
            _students_balls_top.push_back(name);
        }
        else
        {
            _students.find(name)->second.insert(task_number);
        }

        std::find_if(_solutions_qty_per_task.begin(), _solutions_qty_per_task.end(), [&](const std::pair<int, int> &l){
            if (l.first == task_number)
            {
                return true;
            }
            return false;
        })->second++;

        std::sort(_solutions_qty_per_task.begin(), _solutions_qty_per_task.end(), [&](const std::pair<int, int> l, const std::pair<int, int> r) {
            if (l.second == r.second)
            {
                return l.first < r.first;
            }
            return l.second < r.second;
        });

        std::sort(_students_task_qty_top.begin(), _students_task_qty_top.end(), [&](const std::string &l, const std::string &r) {
              if (_students.find(l)->second.size() == _students.find(r)->second.size())
              {
                  return l > r;
              }
              return _students.find(l)->second.size() > _students.find(r)->second.size();
        });

        std::sort(_students_balls_top.begin(), _students_balls_top.end(), [&](const std::string &l, const std::string &r) {
            if (BallsSum(_students.find(l)->second) == BallsSum(_students.find(r)->second))
            {
                return l < r;
            }
            return BallsSum(_students.find(l)->second) < BallsSum(_students.find(r)->second);
        });
    }

    void StudentStatistic(std::ostream &out, const std::string &name) const
    {
        if (_students.find(name) != _students.end())
        {
            out << _students.find(name)->second.size() << " " << BallsSum(_students.find(name)->second) << std::endl;
        }
    }

    void StudentsByTasks(std::ostream &out) const
    {
        for (auto it = _students_task_qty_top.end() - 1; it >= _students_task_qty_top.begin(); it--)
        {
            out << *it << " " << _students.find(*it)->second.size() << std::endl;
        }
    }

    void StudentsByBalls(std::ostream &out) const
    {
        for (auto it = _students_balls_top.begin(); it != _students_balls_top.end(); it++)
        {
            out << *it << " " << BallsSum(_students.find(*it)->second) << std::endl;
        }
    }

    void StudentsMoreThanTasks(std::ostream &out, int count) const
    {
        for (int i = (int)_students_task_qty_top.size() - 1; i >= 0; i--)
        {
            if (_students.find(_students_task_qty_top.at(i))->second.size() >= count)
            {
                out << _students_task_qty_top.at(i) << " " << _students.find(_students_task_qty_top.at(i))->second.size() << std::endl;
            }
        }
    }

    void StudentsMoreThanBalls(std::ostream &out, int balls) const
    {
        for (int i = 0; i < _students_balls_top.size(); i++)
        {
            if (BallsSum(_students.find(_students_balls_top.at(i))->second) >= balls)
            {
                out << _students_balls_top.at(i) << " " << BallsSum(_students.find(_students_balls_top.at(i))->second) << std::endl;
            }
        }
    }

    void TasksBySolutions(std::ostream &out) const
    {
        for (auto it = _solutions_qty_per_task.begin(); it != _solutions_qty_per_task.end(); it++)
        {
            out << it->first << " " << it->second << std::endl;
        }
    }

private:

    std::map<std::string, std::set<int> > _students;

    std::vector<std::string> _students_task_qty_top;
    std::vector<std::string> _students_balls_top;

    std::vector<std::pair<int, int> > _solutions_qty_per_task;
    std::vector<int> _tasks;

    int BallsSum(const std::set<int> &v) const
    {
        int sum = 0;
        for (auto it: v)
        {
            sum += _tasks.at(it - 1);
        }
        return sum;
    }


};

void Session(std::ostream &out, std::istream &in)
{
    Worker worker;

    std::string input;

    int tasks_qty = 0;

    in >> tasks_qty;

    for (int i = 0; i < tasks_qty; i++)
    {
        int balls = 0;
        in >> balls;
        worker.AddTask(balls);
    }

    while (input != "EXIT")
    {
        in >> input;
        std::cout << input << std::endl;
        if (input == "SOLVED")
        {
            in >> input;
            int task_number = 0;
            in >> task_number;
            worker.StudentDidSolveTask(input, task_number);
        }

        if (input == "STUD_STAT")
        {
            in >> input;
            worker.StudentStatistic(out, input);
        }

        if (input == "STUDS_BY_TASKS")
        {
            worker.StudentsByTasks(out);
        }

        if (input == "STUDS_BY_BALLS")
        {
            worker.StudentsByBalls(out);
        }

        if (input == "STUDS_MORE_TASKS")
        {
            int count;
            in >> count;
            worker.StudentsMoreThanTasks(out, count);
        }

        if (input == "STUDS_MORE_BALLS")
        {
            int count;
            in >> count;
            worker.StudentsMoreThanBalls(out, count);
        }

        if (input == "TASKS_BY_SOLUTIONS")
        {
            worker.TasksBySolutions(out);
        }
    }


}

int main()
{
    std::ifstream in("test.txt");
    std::ostringstream out;

    Session(out, in);
    std::cout << "1" << std::endl;
    std::ifstream answer("out.txt");
    std::stringstream answerStream;
    answerStream << answer.rdbuf();


    std::ofstream output("output.txt");
    output << out.str();

    if (answerStream.str() == out.str())
    {
        std::cout << "TEST: PASSED" << std::endl;
    }
    else
    {
        std::cout << "TEST: WRONG ANSWER" << std::endl;
    }
}
