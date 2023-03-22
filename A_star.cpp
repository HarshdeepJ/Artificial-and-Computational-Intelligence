#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <time.h>
#include <stack>
using namespace std;

#define SIZE_OF_MAT 3
string final_state[SIZE_OF_MAT][SIZE_OF_MAT];

int calculate_heuristic(string present_mat[][SIZE_OF_MAT])
{
    int h_r = 0;
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            int pos_x = -1, pos_y = -1;
            for (int k = 0; k < SIZE_OF_MAT; k++)
            {
                for (int l = 0; l < SIZE_OF_MAT; l++)
                {
                    if (present_mat[i][j] == final_state[k][l])
                    {
                        pos_x = k;
                        pos_y = l;
                        break;
                    }
                }
                if (pos_x != -1)
                {
                    break;
                }
            }
            h_r += (abs(i - pos_x) + abs(j - pos_y));
        }
    }
    return h_r;
}

struct node
{
    string matrix[SIZE_OF_MAT][SIZE_OF_MAT];
    int heuristic;
    node *parent;
    vector<node *> children;
    int initialized_when;
    node(string arr[][SIZE_OF_MAT], node *par)
    {
        for (int i = 0; i < SIZE_OF_MAT; i++)
        {
            for (int j = 0; j < SIZE_OF_MAT; j++)
            {
                matrix[i][j] = arr[i][j];
            }
        }
        parent = par;
        
        if (par == nullptr)
        {
            initialized_when = 0;
        }
        else
            initialized_when = par->initialized_when + 1;
        heuristic = calculate_heuristic(arr)+initialized_when;
    }
};



class information
{
public:
    int heuristic;
    int initialized_when;
    node *state;
    information(int h, node *s)
    {
        heuristic = h;
        state = s;
    }
};

struct Comparision
{
    bool operator()(const information &a, information &b) const
    {
        return a.heuristic > b.heuristic;
    }
};

priority_queue<information, vector<information>, Comparision> PQ;

string convert_to_string(string arr[][SIZE_OF_MAT])
{
    string s = "";
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            s += arr[i][j];
        }
    }
    return s;
}

class Tree
{
private:
    node *temp;
    unordered_set<string> visited_states;

public:
    node *start_state;
    Tree(string start_matrix[][SIZE_OF_MAT])
    {
        start_state = new node(start_matrix, nullptr);
        start_state->heuristic = calculate_heuristic(start_matrix);
        PQ.push(information(start_state->heuristic, start_state));
        visited_states.insert(convert_to_string(start_matrix));
    }

    void add_children(string temp_arr[][SIZE_OF_MAT], node *par)
    {
        if (visited_states.find(convert_to_string(temp_arr)) == visited_states.end())
        {
            temp = new node(temp_arr, par);
            par->children.push_back(temp);
            sort(par->children.begin(), par->children.end());
            temp->parent = par;
            visited_states.insert(convert_to_string(temp_arr));
            PQ.push(information(temp->heuristic, temp));
        }
    }
    void what_next(node *parent)
    {
        int pos_of_B_x = -1, pos_of_B_y = -1;
        for (int i = 0; i < SIZE_OF_MAT; i++)
        {
            for (int j = 0; j < SIZE_OF_MAT; j++)
            {
                if (parent->matrix[i][j] == "B")
                {
                    pos_of_B_x = i;
                    pos_of_B_y = j;
                    break;
                }
            }
            if (pos_of_B_x != -1)
            {
                break;
            }
        }

        for (int i = -1; i <= 1; i += 2)
        {
            for (int j = 1; j <= 2; j++)
            {
                string temp_matrix[SIZE_OF_MAT][SIZE_OF_MAT];
                for (int k = 0; k < SIZE_OF_MAT; k++)
                {
                    for (int l = 0; l < SIZE_OF_MAT; l++)
                    {
                        temp_matrix[k][l] = parent->matrix[k][l];
                    }
                }
                int to_swap_x = pos_of_B_x + i * j / 2;
                int to_swap_y = pos_of_B_y + i * j % 2;
                if (pos_of_B_x + i * j / 2 < SIZE_OF_MAT && pos_of_B_y + i * j % 2 < SIZE_OF_MAT && pos_of_B_y + i * j % 2 >= 0 && pos_of_B_x + i * j / 2 >= 0)
                {
                    swap(temp_matrix[pos_of_B_x][pos_of_B_y], temp_matrix[to_swap_x][to_swap_y]);
                    add_children(temp_matrix, parent);
                }
            }
        }
    }
};

void show_the_path(node *final_s)
{
    stack<node *> path;
    node *states = final_s;
    while (final_s != nullptr)
    {
        path.push(final_s);
        final_s = final_s->parent;
    }
    while (!path.empty())
    {
        for (int i = 0; i < SIZE_OF_MAT; i++)
        {
            for (int j = 0; j < SIZE_OF_MAT; j++)
            {
                cout << path.top()->matrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        path.pop();
    }
}

int main()
{
    int count = 0;
    freopen("Output A.txt", "w", stdout);
    string start_matrix[SIZE_OF_MAT][SIZE_OF_MAT];
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            cin >> start_matrix[i][j];
        }
    }
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            cin >> final_state[i][j];
        }
    }
    clock_t tStart = clock();
    Tree t(start_matrix);

    while (!PQ.empty())
    {
        if (PQ.top().heuristic-PQ.top().state->initialized_when == 0)
        {
            cout << "Successfully reached final state.\n"
                 << endl;
            cout << "Initial State: " << endl;
            for (int i = 0; i < SIZE_OF_MAT; i++)
            {
                for (int j = 0; j < SIZE_OF_MAT; j++)
                {
                    cout << start_matrix[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;
            cout << "Final State: " << endl;
            for (int i = 0; i < SIZE_OF_MAT; i++)
            {
                for (int j = 0; j < SIZE_OF_MAT; j++)
                {
                    cout << PQ.top().state->matrix[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;
            cout << "Number of states explored: " << count << endl;
            cout << "Total number of states to optimal path: " << PQ.top().state->initialized_when << endl;
            cout << "\nOptimal path to the final state: " << endl;
            show_the_path(PQ.top().state);
            cout << "Time taken for execution: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;
            return 0;
        }
        count++;
        node *temp_state = PQ.top().state;
        PQ.pop();
        t.what_next(temp_state);
    }
    cout << "Didn't reach the final state \n"<< endl;
    cout << "Initial State: " << endl;
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            cout << start_matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Final State: " << endl;
    for (int i = 0; i < SIZE_OF_MAT; i++)
    {
        for (int j = 0; j < SIZE_OF_MAT; j++)
        {
            cout << PQ.top().state->matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Number of states explored: " << count << endl;
}