// Author - Aman-Rohilla@rohilla.co.in

#ifndef UTILS__H
#define UTILS__H

#define _WIN32_WINNT 0x0600
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#pragma comment(lib, "Wininet.lib")
#include <wininet.h>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>

#include <shlwapi.h>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <windows.h>
#include <jsoncpp/json.h>
#include <jsoncpp/reader.h>
#include <jsoncpp/writer.h>
#include <jsoncpp/value.h>
#include <math.h>
#include <cctype>

#define lpn string::npos
using namespace std;

HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
int argcg;
char **argvg;

bool str_find(string s, string str, size_t i = 0)
{
    if (s.find(str, i) != string::npos)
        return 1;

    return 0;
}

size_t str_find_index(string &s, string str, size_t i = 0, int reverse = 0)
{
    size_t f;
    if (!reverse)
        f = s.find(str, i);
    else
        f = s.rfind(str, i);
    if (f != string::npos)
        return f;

    return -1;
}
int str_find_count(string s, string str, size_t f = 0)
{
    f = s.find(str, f);
    if (f == string::npos)
        return 0;
    int count = 0;
    while (f != string::npos)
    {
        count++;
        f = s.find(str, f + 1);
    }
    return count;
}

string erase_find(string s, string str)
{
    size_t f = s.find(str);
    if (f != string::npos)
        s.erase(0, f + 1);
    return s;
}

string erase_find_found_to_end(string s, string str)
{
    size_t f = s.find(str);
    if (f != string::npos)
        s.erase(f, s.length() - f);

    return s;
}

string erase_rfind(string s, string str)
{
    size_t f = s.rfind(str);
    if (f != string::npos)
        s.erase(0, f + 1);
    return s;
}

string erase_rfind_found_to_end(string s, string str)
{
    size_t f = s.rfind(str);
    if (f != string::npos)
        s.erase(f, s.length() - f);

    return s;
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        COORD invalid = {0, 0};
        return invalid;
    }
}

int current_console_cursor(int &x, int &y, int set = 0)
{
    COORD c; // = {a,b};
    if (!set)
    {
        c = GetConsoleCursorPosition(hstdout);
        x = c.Y;
        y = c.X;
        return 1;
    }
    else
    {
        c.X = y;
        c.Y = x;
        bool b = SetConsoleCursorPosition(hstdout, c);
        return b;
    }
}

void setConsoleColor(WORD c)
{
    SetConsoleTextAttribute(hstdout, c);
}
string add_gaps(string s, int n = 0, int front = 0, string gap = " ")
{
    int gaps = n - s.length();
    for (int i = 0; i < gaps; i++)
    {
        if (front)
            s = gap + s;
        else
            s += gap;
    }
    return s;
}
template<typename T>
void cout_t(T value) {
    cout<<endl<<value<<endl;
}

template<typename T>
void cout_v(string name, T value, int gaps=10) {
    cout<<endl<<add_gaps(name, gaps, 1)<<" : "<<value<<endl;
}

template <typename T>
string conv2str(T var)
{
    stringstream ss;
    ss << var;
    return ss.str();
}

float float_precision(float f, int precision = 2)
{
    stringstream tmp;
    tmp << setprecision(precision) << fixed << f;
    return stof(tmp.str());
}

double double_precision(double f, int precision = 2)
{
    stringstream tmp;
    tmp << setprecision(precision) << fixed << f;
    return stod(tmp.str());
}


void console_cursor(bool show)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hstdout, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(hstdout, &cursorInfo);
}

size_t inline MB2B(float size)
{
    return 1000 * 1000 * size;
}

bool exists(string s)
{
    if (s == "")
        return 0;
    return PathFileExistsA(s.c_str());
}

string del_ex(string in)
{
    string s = in;
    size_t f = s.rfind(".");
    if (f == string::npos)
        return s;

    s.erase(f, s.length() - f);
    return s;
}

string ext_ex(string in, int dot = 1)
{
    string s = in;
    size_t f = s.rfind(".");
    if (f == string::npos)
        return "";

    if (dot)
        s.erase(0, f);
    else
        s.erase(0, f + 1);
    return s;
}

string ext_dn(string s)
{
    size_t f = s.rfind("\\");
    if (f != string::npos)
        s.erase(f + 1, s.length() - f - 1);
    else
        s = "";
    return s;
}
string ext_fn(string s)
{
    size_t f = s.rfind(".");
    if(f!=string::npos) s.erase(f,s.length()-f);

    f = s.rfind("\\");
    if(f!=string::npos) s.erase(0,f+1);
    return s;
}

string exe_path()
{
    char result[MAX_PATH];
    return string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}

string out__file(string s, string app)
{
    return del_ex(s) + app + ext_ex(s);
}

string name_assigner(string s)
{
    if (!exists(s))
        return s;

    int i = 2;
    while (exists(out__file(s, "_" + to_string(i))))
        i++;
    return out__file(s, "_" + to_string(i));
}

string replace_string_all(string s, string old, string new_s, int total = 0)
{
    size_t f = s.find(old);
    while (f != string::npos)
    {
        s.replace(f, old.length(), new_s);
        if (!total)
            f = s.find(old, f + new_s.length());
        else
            f = s.find(old);
    }
    return s;
}

string validate_path(string s)
{
    // Invalid chars <>:"/\|?*
    // integer value 127
    // Integer value 0 to 32, -256 to -224

    if (s.length() == 0)
        return "";

    s = replace_string_all(s, "\n", "_");
    s = replace_string_all(s, "\r", "_");

    int len = s.length();
    vector<int> arr = {-256, -255, -254, -253, -252, -251, -250, -249, -248, -247, -246, -245, -244, -243,
                       -242, -241, -240, -239, -238, -237, -236, -235, -234, -233, -232, -231, -230, -229,
                       -228, -227, -226, -225, -224, -222, -214, -209, -198, -196, -194, -193, -164, -132,
                       -129, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
                       24, 25, 26, 27, 28, 29, 30, 31, 34, 42, 47, 58, 60, 62, 63, 124, 127};

    // char chars[] = {'<','>',':','"','/','\\','|','?','*'};
    for (int i = 0; i < len; i++)
        if (binary_search(arr.begin(), arr.end(), (int)s[i]))
        {
            if (s[i] == ':' && s[1] == ':' && s[2] == '\\')
                continue;
            s.replace(i, 1, "_");
        }

    len = s.length();

    if (s[len - 1] == ' ' || s[len - 1] == '.')
        s.replace(len - 1, 1, "_");

    return s;
}

string lower_case_converter(string s)
{
    int len = s.length();
    if (!len)
        return s;

    for (int i = 0; i < len; i++)
        if (s[i] <= 'Z' && s[i] >= 'A')
            s[i] = (char)((int)s[i] + 32);

    return s;
}


string text_to_string(string file)
{
    if (!exists(file))
        return "";
    string s;

    ifstream text(file);
    stringstream ss;
    ss << text.rdbuf();
    text.close();
    s = ss.str();
    return s;
}

int lines_of_text(string s, int f = 1)
{
    ifstream in(s);
    if (!in)
        return -1;
    int lines = 0;

    while (!in.eof())
    {
        getline(in, s);
        if (f == 1 && s.length() == 0)
            continue;
        lines++;
    }
    return lines;
}

template <typename T>
void write_to_text(string file, T s, int append = 1, int new_line = 1)
{
    int lines = lines_of_text(file);
    if (lines == -1)
        lines = 0;

    if (append)
    {
        ofstream out(file, ios_base::app | ios_base::ate);
        if (new_line && lines)
            out << endl;
        out << s;
        out.close();
    }
    else
    {
        ofstream out(file);
        out << s;
        out.close();
    }
    return;
}

string pipe_open(string s, int *retval = NULL, int std_err = 0, int capture = 1, int std_err_nul = 1, string *error = NULL)
{
    char buffer[4096];
    FILE *pipe;

    string tf;

    if (std_err_nul)
        s += " 2>nul";
    else if (std_err && !error)
        s += " 2>&1";
    else if (std_err && error)
    {
        tf = name_assigner("temp");
        s += " 2>" + tf;
    }

    if ((pipe = _popen(s.c_str(), "rt")) == NULL)
    {
        if (retval)
            *retval = -1;
        return "";
    }

    string str;
    while (fgets(buffer, 4096, pipe))
    {
        if (capture)
            str += buffer;
    }

    if (feof(pipe) && retval)
        *retval = _pclose(pipe);

    int i = 1;
    if (feof(pipe))
        i = 1;
    if (tf != "")
        *error = text_to_string(tf);

    return str;
}

struct spy_tag__
{
    string album;
    string album_url;
    string cover;
    string date;
    string title;
    string url;
    string artists;
    string duration;
    string isrc;
    string playlist;
    string playlist_url;
    string albumartists;
    string temp;
    int tn;
    int nos;
    int sn;
    string get(string sep = "|", string print = "")
    {
        string s = album + sep + title + sep + artists + sep + albumartists
        + sep + date + sep + to_string(tn) + sep + to_string(nos)
        + sep + duration + sep + isrc + sep + album_url + sep + url + sep + cover;
        if (print != "")
            cout << s << print;
        return s;
    }
    void replace_pipe()
    {
        album = replace_string_all(album, "|", "--pipe");
        album_url = replace_string_all(album_url, "|", "--pipe");
        title = replace_string_all(title, "|", "--pipe");
        artists = replace_string_all(artists, "|", "--pipe");
        albumartists = replace_string_all(albumartists, "|", "--pipe");
    }
};

string copy_str(string &s, size_t i, size_t j)
{
    string rs;
    for (size_t k = i; k <= j; k++)
        rs.push_back(s[k]);
    return rs;
}
int str2json(string &str, Json::Value &root, string *errors = NULL)
{
    string js;
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    bool b = reader->parse(str.c_str(), str.c_str() + str.length(), &root, errors);
    delete reader;

    return b;
}

string get_column(string s, int col, char c = '|')
{
    if (col < 0 || s == "")
        return "";

    stringstream ss = stringstream(s);

    int i = 0;
    while (!ss.eof())
    {
        i++;
        getline(ss, s, c);
        if (i == col)
            return s;
    }

    return "";
}

void psv_sorter_vec(vector<string> &v, int col = 0, int reverse = 0)
{
    size_t n = v.size();
    if (n < 2)
        return;

    sort(v.begin(), v.end(), [col, reverse](string &s1, string &s2)
         {
        if(!reverse)
            return get_column(s1,col) < get_column(s2,col);

        return get_column(s1,col) > get_column(s2,col); });
}

void console_pointer_code(string s)
{
    if (s.length() == 0)
        return;

    DWORD mode = 0;
    if (!GetConsoleMode(hstdout, &mode))
        return;

    const DWORD original_mode = mode;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hstdout, mode))
    {
        return;
    }

    // Write the sequence for clearing the display.
    DWORD written = 0;

    string cmd;

    if (s == "up")
        cmd = "\033[A";
    else if (s == "b")
        cmd = "\b";
    else if (s == "el")
        cmd = "\33[2K";
    else if (s == "els")
        cmd = "\33[2K\r";
    else if (s == "sl")
        cmd = "\r";
    else if (s == "cl")
        cmd = "\33[2J";

    PCWSTR str = (std::wstring(cmd.begin(), cmd.end())).c_str();

    if (cmd != "" && !WriteConsoleW(hstdout, str, (DWORD)wcslen(str), &written, NULL))
    {
        SetConsoleMode(hstdout, original_mode);
        return;
    }

    SetConsoleMode(hstdout, original_mode);
}

int is_int(string s)
{
    int len = s.length();
    if (!len)
        return 0;
    for (int i = 0; i < len; i++)
        if (s[i] < '0' || s[i] > '9')
            return 0;
    return 1;
}

int s2i(string s, int *n)
{
    if (is_int(s))
    {
        if (n)
            *n = stoi(s);
        return 1;
    }

    return 0;
}
void console_pointer(string s, int skip = -1);

void erase_console_line(int X, int skip = -1)
{
    if (X < 0)
        return;
    int x, y;

    current_console_cursor(x, y);
    while (x >= X)
    {
        current_console_cursor(x, y, 1);
        if (skip != x)
            console_pointer("el");
        x--;
    }
}

void console_pointer(string s, int skip)
{
    if (!s.length())
        return;
    s = replace_string_all(s, "  ", " ");

    stringstream ss = stringstream(s);

    while (!ss.eof())
    {
        getline(ss, s, ' ');
        if (s == "ml")
        {
            getline(ss, s, ' ');
            int left;

            if (s2i(s, &left))
            {
                int x, y;
                current_console_cursor(x, y);
                y = y - left;
                if (y < 0)
                    y = 0;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "mr")
        {
            getline(ss, s, ' ');
            int right;
            if (s2i(s, &right))
            {
                int x, y;
                current_console_cursor(x, y);
                y = y + right;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "mu")
        {
            getline(ss, s, ' ');
            int up;
            if (s2i(s, &up))
            {
                int x, y;
                current_console_cursor(x, y);
                x = x - up;
                if (x < 0)
                    x = 0;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "md")
        {
            getline(ss, s, ' ');
            int down;
            if (s2i(s, &down))
            {
                int x, y;
                current_console_cursor(x, y);
                x = x + down;
                current_console_cursor(x, y, 1);
            }
        }
        else if (s == "cl")
        {
            erase_console_line(0);
        }
        else if (s == "etl")
        {
            getline(ss, s, ' ');
            int x;
            if (s2i(s, &x))
                erase_console_line(x, skip);
        }
        else if (s == "etls")
        {
            getline(ss, s, ' ');
            int x;
            if (s2i(s, &x))
                erase_console_line(x, skip);
            console_pointer("sl");
        }
        else if (s == "st")
        {
            int x = 0, y = 0;
            current_console_cursor(x, y, 1);
        }
        else if (s == "ess")
        {
            console_pointer("cl st");
        }
        else if (s == "sp")
        {
            int x, y;
            getline(ss, s, ' ');

            if (s2i(s, &x))
            {
                getline(ss, s, ' ');
                if (s2i(s, &y))
                    current_console_cursor(x, y, 1);
            }
        }
        else
            console_pointer_code(s);
    }
}

int get_console_rc(int col = 1)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);
    if (col)
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    else
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void erase_console(int X, int Y, int skip = -1)
{
    if (X < 0 || Y < 0)
        return;
    int x, y;

    current_console_cursor(x, y);
    while (x >= X + 1)
    {
        current_console_cursor(x, y, 1);
        if (skip != x)
            console_pointer("el");
        x--;
    }
    int len = get_console_rc() - Y;

    string s;
    for (int i = 0; i < len; i++)
        s += " ";

    current_console_cursor(X, Y, 1);
    cout << s;
    current_console_cursor(X, Y, 1);
}

struct codepage__
{
    int cp;
    codepage__()
    {
        cp = GetConsoleOutputCP();
    }
    void unicode()
    {
        SetConsoleOutputCP(65001);
    }
    void restore()
    {
        SetConsoleOutputCP(cp);
    }
};

struct progress_bar_param
{
    int execute = 1;
    int process = 0;
    int perform = 0;
    std::future<int> rv;
    int current_count = 0;
    int total_count = 0;
    int current = 0;
    int started = 0;
    int max_len = 0;
    int prog_start_x = 0;
    int start_x = 0;
    int start_y = 0;
    int start_X = 0;
    int start_Y = 0;
    int end_x = 0;
    int end_y = 0;
    int end_X = 0;
    int end_Y = 0;
    int once = 0;
    int new_line_len = 0;
    int new_line_x = 0;
    int new_line_y = 0;
    int info_len = 0;
    int mode = 0;
    int async = 1;
    int active = 0;
    int exit = 0;
    int running = 0;
    codepage__ cp;
    /// above variables are internal to progress bar

    string task;
    string extra_info;
    string new_line;

    int precision = 0;
    int set_len = 0;
    int clear = 1;
    int percentage_gap = 1;
    int resize = 1;
    int display = 1;
    int cursor = 0;
    int console_size = 0;
    int console_height = 0;
    int wide_chars = 1;
    int bell = 0;
    int pause = 0;
    int paused = 0;
    int organize = 1;
    int ended = 1;
    int first_line = 1;
    int thread = 0;
    int delay = 50;
    int delay_max = 500;
    int delay_min = 20;
    int save_state = 0;
    int delay_clear = 1000;
    int cleared = 0;
    int blocks = 50;
    float interval = 1;
    int ran = 0;

    string completed = "█";
    string remaining = "░";

    string tab = "        ";
    string start_lines;
    string task_lines = "\n\n";
    string new_lines = "\n\n";
    string end_lines = "\n\n";
    string transition = " -> ";

    progress_bar_param(string task_ = "");

    void launch();
    void relaunch();
    void end();
    void update(int comp = 0, int thread_ = 0);
    void terminate(string s = "");
};
int progress_bar_async(progress_bar_param *pb);
void progress_bar(progress_bar_param &pb, int comp = 0);

progress_bar_param::progress_bar_param(string task_)
{
    int y;
    current_console_cursor(this->prog_start_x, y);
    task = task_;
}

void progress_bar_param::relaunch()
{
    if (!this->execute)
        return;
    this->terminate();
    this->launch();
}

void progress_bar_param::end()
{
    cout.flush();
    if (!this->execute)
        return;

    if (!this->ended && !this->save_state)
    {
        if (this->wide_chars)
        {
            this->cp.restore();
            this->mode = 0;
        }

        if (this->new_line_x > this->start_x || (this->new_line_x == this->start_x && this->new_line_y > this->start_y))
            current_console_cursor(this->new_line_x, this->new_line_y, 1);

        if (this->clear && this->started)
        {
            Sleep(this->delay_clear);
            erase_console(this->start_X, this->start_Y);
            this->end_X = this->start_X;
            this->end_Y = this->start_Y;
        }
        else
            cout << this->end_lines;

        if (this->clear)
            this->once = 0;
        else
            this->once = 1;
        console_cursor(1);
        if (this->bell)
            cout << "\a";

        current_console_cursor(this->end_X, this->end_Y);

        this->extra_info = "";
        this->new_line = "";

        this->current_count = 0;
        this->total_count = 0;
        this->current = 0;
        this->started = 0;
        this->new_line_len = 0;
        this->info_len = 0;
        this->process = 0;
        this->active = 0;
        this->cleared = 0;
        this->ended = 1;
    }
}

void progress_bar_param::terminate(string s)
{
    cout.flush();

    if (this->ran)
    {
        this->ran = 0;
        if (!this->ended)
            this->update(1);
    }

    if (this->running)
    {
        this->exit = 1;
        while (this->running)
            Sleep(this->delay);
    }
    else
        this->end();

    if (s == "up")
    {
        if (this->organize)
        {
            int x, y;
            current_console_cursor(x, y);
            if (!this->clear && x == this->end_X && y == this->end_Y && this->end_lines.find("\n\n") != string::npos)
                console_pointer("up");
        }
    }
}

void progress_bar_param::launch()
{
    if (!this->execute || !this->thread)
        return;
    if (!this->running)
    {
        this->rv = std::async(std::launch::async, progress_bar_async, this);
        while (!this->running)
            Sleep(this->delay);
    }
}

void progress_bar_param::update(int comp, int thread_)
{
    cout.flush();
    if (!this->execute)
        return;
    if (!this->ran)
        ran = 1;

    if (this->running)
    {
        if (comp)
            this->perform = 1;
        else
            this->perform = 0;
        this->process++;
        return;
    }

    if (thread_)
        thread = 1;

    if (this->thread)
    {
        this->launch();
        if (comp)
            this->perform = 1;
        else
            this->perform = 0;
        this->process++;
    }
    else
        progress_bar(*this, comp);
}

string fill_gap(string s, int n)
{
    int len = s.length();
    for (int i = 0; i < (n - len); i++)
        s.push_back(' ');
    return s;
}
int no_console_gaps(int a1, int b1, int a2, int b2, int size)
{
    if (a1 == a2 && b1 == b2)
        return 0;
    if (size == 0)
        return 0;

    int gaps = 0;

    int x = abs(a2 - a1);
    int i = 0;

    while (i < x)
    {
        gaps += size;
        i++;
    }
    gaps += abs(b2 - b1);
    return gaps;
}

void progress_bar(progress_bar_param &pb, int comp)
{
    if (!pb.execute)
        return;
    if (!pb.total_count || !pb.execute)
        return;
    int p;
    string s, pbcs, pbrs;
    string info;

    pbcs = pb.completed;
    pbrs = pb.remaining;
    int pbcc = pb.current_count;
    if (pbcc && !comp)
        pbcc--;

    float pp = ((float)(pbcc * 100)) / pb.total_count;
    if (pp > 100)
        pp = 100;
    p = (int)pp;
    if (pp - p > 0.5 && p != 99)
        p++;

    if (!pb.started)
    {
        pb.end();
        pb.ended = 0;

        string pbbc[] = {"▌", "█"};

        sort(pbbc, pbbc + 2);

        if (binary_search(pbbc, pbbc + 2, pb.completed))
        {
            pb.completed = "█";
            pbcs = pb.completed;
        }
        pb.max_len = (pb.task + to_string(pb.total_count) + "/" + to_string(pb.total_count)).length();

        if (pb.wide_chars)
        {
            pb.cp.unicode();
            pb.mode = 1;
        }

        int x, y;
        pb.current = -1;
        current_console_cursor(x, y);
        pb.start_X = x;
        pb.start_Y = y;

        if (pb.organize && x == pb.prog_start_x && pb.first_line)
        {
            cout << endl;
            x++;

            if (y)
            {
                cout << endl;
                x++;
            }
            y = 0;
        }
        else if (pb.organize && x && pb.end_X)
        {
            if (x == pb.end_X && !y)
            {
                x++;
                cout << endl;
                y = 0;
            }
            else if (x == pb.end_X || y)
            {
                cout << endl
                     << endl;
                x += 2;
                y = 0;
            }
        }

        pb.start_x = x;
        pb.start_y = y;
        if (!pb.cursor)
            console_cursor(0);
    }

    if (pb.current_count <= pb.current && pb.task == "" && pb.new_line == "" && pb.extra_info == "" && !comp)
        return;

    float per = 100.0 / pb.blocks;
    int chars = pp / per;

    for (int k = 0; k < chars; k++)
        s += pbcs;

    if (pb.completed == "█")
    {
        float fx = fmod(pp, per);
        if (fx >= per / 2)
        {
            chars++;
            s += "▌";
        }
    }

    for (int i = 0; i < pb.blocks - chars; i++)
        s += pbrs;

    s = pb.task_lines + pb.tab + s + " ";
    if (p < 10)
        s += "  ";
    else if (p < 100)
        s += " ";
    s += to_string(p);

    if (pb.percentage_gap)
        s += " ";
    s = s + "%";

    int console_cols = get_console_rc();

    if (pb.cleared == 1)
    {
        erase_console(pb.start_x, pb.start_y);
        pb.cleared = 0;
    }

    current_console_cursor(pb.start_x, pb.start_y, 1);

    if (pb.task != "" || pb.extra_info != "")
    {
        if (pb.task != "" && pb.extra_info != "")
        {
            if (!pb.set_len)
                info = fill_gap(pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count), pb.max_len) + pb.extra_info;
            else
                info = fill_gap(pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count), pb.set_len) + pb.extra_info;
        }
        else if (pb.task != "")
            info = pb.task + to_string(pb.current_count) + "/" + to_string(pb.total_count);
        else if (pb.extra_info != "")
            info = pb.extra_info;

        info = replace_string_all(info, "\t", pb.tab);
        info = replace_string_all(info, "\n", "");

        int last_info_len = pb.info_len;

        pb.info_len = info.length();

        if (info.length() > console_cols)
            info.erase(console_cols, info.length() - console_cols);
        else if (pb.started && last_info_len > pb.info_len)
            info = add_gaps(info, console_cols);

        s = info + s;
    }

    cout << s;

    if (pb.new_line != "")
        cout << pb.new_lines << pb.new_line;

    current_console_cursor(pb.new_line_x, pb.new_line_y);

    if (!pb.cleared && pb.started && (pb.end_x > pb.new_line_x || (pb.end_x == pb.new_line_x && pb.end_y > pb.new_line_y)))
    {
        int gap_len = no_console_gaps(pb.new_line_x, pb.new_line_y, pb.end_x, pb.end_y, console_cols);
        string g;
        for (int i = 0; i < gap_len; i++)
            g += " ";
        cout << g;
    }

    pb.end_x = pb.new_line_x;
    pb.end_y = pb.new_line_y;
    pb.end_X = pb.new_line_x;
    pb.end_Y = pb.new_line_y;
    pb.current = pb.current_count;
    pb.started = 1;
    if (pb.current_count == pb.total_count && comp)
        pb.end();
}

int progress_bar_async(progress_bar_param *pb)
{
    int flfl = 0, process = 0, flag = 0;
    pb->running = 1;
    if (pb->delay < pb->delay_min)
        pb->delay = pb->delay_min;
    if (pb->delay > pb->delay_max)
        pb->delay = pb->delay_max;

    while (pb->async == 0)
    {
        if (pb->exit)
            break;
        Sleep(pb->delay);
    }

    if (!pb->exit && pb->execute)
    {
        pb->active = 1;
        while (1)
        {
            if (!pb->exit)
            {
                if (!pb->total_count)
                    continue;

                if (pb->paused)
                    continue;
                else if (pb->pause)
                {
                    pb->paused = 1;
                    pb->pause = 0;
                    continue;
                }
            }

            if (process < pb->process)
            {
                process = pb->process;
                if (!flag)
                    flag = 1;
            }
            else if (!pb->exit)
                continue;

            if (flag)
            {
                int p;
                string s, pbcs, pbrs, extra_info, new_line;
                string info;

                pbcs = pb->completed;
                pbrs = pb->remaining;
                int current_count = pb->current_count;
                extra_info = pb->extra_info;
                new_line = pb->new_line;
                int pbcc = current_count;
                if (!pb->perform && pbcc)
                    pbcc--;

                float pp = ((float)(pbcc * 100)) / pb->total_count;
                if (pp > 100)
                    pp = 100;
                p = (int)pp;
                if (pp - p > 0.5 && p != 99)
                    p++;

                if (!pb->started && !pb->save_state)
                {
                    pb->end();
                    pb->ended = 0;

                    string pbbc[] = {"▌", "█"};

                    sort(pbbc, pbbc + 2);

                    if (binary_search(pbbc, pbbc + 2, pb->completed))
                    {
                        pb->completed = "█";
                        pbcs = pb->completed;
                    }

                    pb->max_len = (pb->task + to_string(pb->total_count) + "/" + to_string(pb->total_count)).length();

                    if (pb->wide_chars)
                    {
                        pb->cp.unicode();
                        pb->mode = 1;
                    }
                    int x, y;
                    pb->current = -1;
                    current_console_cursor(x, y);
                    pb->start_X = x;
                    pb->start_Y = y;

                    if (pb->organize && x == pb->prog_start_x && pb->first_line)
                    {
                        cout << endl;
                        x++;

                        if (y)
                        {
                            cout << endl;
                            x++;
                        }
                        y = 0;
                    }
                    else if (pb->organize && x && pb->end_X)
                    {
                        if (x == pb->end_X && !y)
                        {
                            x++;
                            cout << endl;
                            y = 0;
                        }
                        else if (x == pb->end_X || y)
                        {
                            cout << endl
                                 << endl;
                            x += 2;
                            y = 0;
                        }
                    }
                    pb->start_x = x;
                    pb->start_y = y;
                    if (!pb->cursor)
                        console_cursor(0);
                }

                if (current_count <= pb->current && pb->task == "" && new_line == "" && extra_info == "" && !pb->exit && !pb->perform)
                    continue;

                float per = 100.0 / pb->blocks;
                int chars = pp / per;

                for (int k = 0; k < chars; k++)
                    s += pbcs;

                if (pb->completed == "█")
                {
                    float fx = fmod(pp, per);
                    if (fx >= per / 2)
                    {
                        chars++;
                        s += "▌";
                    }
                }

                for (int i = 0; i < pb->blocks - chars; i++)
                    s += pbrs;

                s = pb->task_lines + pb->tab + s + " ";
                if (p < 10)
                    s += "  ";
                else if (p < 100)
                    s += " ";
                s += to_string(p);

                if (pb->percentage_gap)
                    s += " ";
                s = s + "%";

                if (pb->task != "" || extra_info != "")
                {
                    if (pb->task != "" && extra_info != "")
                    {
                        if (!pb->set_len)
                            info = fill_gap(pb->task + to_string(current_count) + "/" + to_string(pb->total_count), pb->max_len) + extra_info;
                        else
                            info = fill_gap(pb->task + to_string(current_count) + "/" + to_string(pb->total_count), pb->set_len) + extra_info;
                    }
                    else if (pb->task != "")
                        info = pb->task + to_string(current_count) + "/" + to_string(pb->total_count);
                    else if (extra_info != "")
                        info = extra_info;

                    info = replace_string_all(info, "\t", pb->tab);
                    info = replace_string_all(info, "\n", "");

                    int last_info_len = pb->info_len;

                    pb->info_len = info.length();
                    s = info + s;
                }
                current_console_cursor(pb->start_x, pb->start_y, 1);

                if (pb->cleared == 1)
                {
                    erase_console(pb->start_x, pb->start_y);
                    pb->cleared = 0;
                }

                current_console_cursor(pb->start_x, pb->start_y, 1);
                cout << s;

                if (new_line != "")
                    cout << pb->new_lines << new_line;

                current_console_cursor(pb->new_line_x, pb->new_line_y);

                pb->end_x = pb->new_line_x;
                pb->end_y = pb->new_line_y;
                pb->end_X = pb->new_line_x;
                pb->end_Y = pb->new_line_y;
                pb->current = current_count;
                pb->started = 1;
                if (current_count >= pb->total_count && pb->perform)
                    break;
            }
            if ((pb->exit || pb->current_count == pb->total_count) && !flfl)
                flfl = 1;
            else if (pb->exit)
                break;
            Sleep(pb->delay);
        }
        if (!pb->save_state)
            pb->end();
    }

    pb->exit = 0;
    pb->process = 0;
    pb->running = 0;
    return 0;
}

int keypress(int k1, int k2 = 0, int k3 = 0, int k4 = 0, int *flag = NULL, int delay = 50)
{
    int k = k1;
    if (k4)
        k = k4;
    else if (k3)
        k = k3;
    else if (k2)
        k = k2;

    if ((GetAsyncKeyState(k1) & 0x8000) && (!k2 || (GetAsyncKeyState(k2) & 0x8000)) &&
        (!k3 || (GetAsyncKeyState(k3) & 0x8000)) && (!k4 || (GetAsyncKeyState(k4) & 0x8000)))
    {
        while (GetAsyncKeyState(k) != 0)
            Sleep(delay);
        return 1;
    }
    return 0;
}

struct pause_param
{
    int execute = 1;
    std::future<int> rv;
    int async = 1;
    int running = 0;
    int stop = 0;
    int exit = 0;
    int exited = 0;
    int started = 0;
    int stopped = 0;
    int delay = 50;
    int delay_max = 500;
    int delay_min = 20;
    int paused = 0;
    int resuming = 0;
    int delayed_message = 0;
    int message = 0;
    int k1 = VK_CONTROL;
    int k2 = VK_SHIFT;
    int k3 = 0;
    int k4 = 0;
    int stop_k1 = VK_CONTROL;
    int stop_k2 = VK_END;
    int once = 0;
    int current = 0;
    int resume_delay = 100;
    int messaging = 0;
    int STOP = 0;
    progress_bar_param *pb = NULL;
    string title = "";
    HWND window = GetForegroundWindow();

    pause_param(progress_bar_param *pbp, string title_ = "");

    void launch();
    void terminate();
    void relaunch();
    int resume();
};

pause_param::pause_param(progress_bar_param *pbp, string title_)
{
    pb = pbp;
    title = title_;
}

void pause_param::terminate()
{
    cout.flush();
    if (!this->execute)
        return;
    if (this->running)
    {
        this->exit = 1;
        while (this->running)
            Sleep(this->delay);
    }
}
void pause_param::relaunch()
{
    if (!this->execute)
        return;
    this->terminate();
    this->launch();
}

int pause_param::resume()
{
    if (!this->execute || (!this->paused && !this->STOP))
        return 0;
    int retval = 0;
    this->resuming = 1;
    this->once = 0;
    this->current = 0;

    if (this->pb)
    {
        if (this->pb->running)
        {
            this->pb->save_state = 1;
            this->pb->terminate();
        }
    }

    int x = 0, y = 0;

    if (!this->STOP)
    {
        current_console_cursor(x, y);

        if (this->pb)
        {
            if (x < this->pb->end_X || (x == this->pb->end_X && y < this->pb->end_Y))
            {
                x = this->pb->end_X;
                y = this->pb->end_Y;
                current_console_cursor(x, y, 1);
            }
        }

        if (y != 0)
            cout << "\n\n\t";
        else
            cout << "\n\t";

        setConsoleColor(176);
        cout << "   Paused...  ";
        while (this->messaging)
            Sleep(this->resume_delay);
    }

    this->terminate();
    if (!this->STOP)
        while (1)
        {
            if (this->window == GetForegroundWindow())
                if (keypress(this->stop_k1, this->stop_k2))
                {
                    retval = 1;
                    break;
                }

            if (this->window == GetForegroundWindow())
                if (keypress(this->k1, this->k2, this->k3, this->k4))
                    break;

            Sleep(this->resume_delay);
        }

    this->launch();
    if (this->pb)
    {
        this->pb->save_state = 0;
        if (this->pb->thread)
            this->pb->launch();
    }

    if (!this->STOP)
    {
        setConsoleColor(7);
        erase_console(x, y);
    }
    else
    {
        this->STOP = 0;
        retval = 1;
    }
    this->paused = 0;
    this->resuming = 0;
    if (this->pb)
    {
        this->pb->paused = 0;
        this->pb->pause = 0;
    }
    cout.flush();
    return retval;
}

int pause(pause_param *pp);

void pause_param::launch()
{
    if (!this->execute)
        return;
    if (!this->running)
    {
        this->rv = std::async(std::launch::async, pause, this);
        while (!this->running)
            Sleep(this->delay);
    }
}

void message_box(string m, string prog_name = "")
{
    MessageBoxA(NULL, m.c_str(), prog_name.c_str(), MB_OK);
}

int pause(pause_param *pp)
{
    if (!pp->execute)
        return 0;
    pp->running = 1;

    if (pp->delay > 200)
        pp->delay = 200;
    else if (pp->delay < 20)
        pp->delay = 20;

    while (pp->async == 0)
    {
        Sleep(pp->delay);
        if (pp->exit)
            break;
    }

    string s;

    while (!pp->exit)
    {
        if (pp->resuming || pp->STOP)
            continue;

        if (pp->window == GetForegroundWindow())
        {
            pp->STOP = keypress(pp->stop_k1, pp->stop_k2);
            if (pp->STOP)
                continue;
        }

        if (pp->window == GetForegroundWindow())
            pp->current = keypress(pp->k1, pp->k2, pp->k3, pp->k4);

        if (pp->current)
            pp->paused = 1;

        if (!pp->paused)
        {
            pp->current = 0;
            pp->once = 0;
        }

        if (pp->delayed_message && pp->current)
        {
            if (!pp->once)
            {
                pp->messaging = 1;
                message_box("Pause request...\n\nUnable to pause right now,\nwill try whenever possible", pp->title);
                pp->messaging = 0;
                pp->current = 0;
                pp->once = 1;
            }
            else
            {
                pp->paused = 0;
                pp->messaging = 1;
                message_box("Resume Request...\n\nEarliar Pause request dismissed", pp->title);
                pp->messaging = 0;
                pp->current = 0;
                pp->once = 0;
            }
        }
        else if (pp->message && pp->current)
        {
            if (!pp->once)
            {
                pp->messaging = 1;
                message_box("Pause request...\n\n", pp->title);
                pp->messaging = 0;
                pp->current = 0;
                pp->once = 1;
            }
            else
            {
                pp->paused = 0;
                pp->messaging = 0;
                message_box("Resume Request...\n\nEarliar Pause request dismissed", pp->title);
                pp->messaging = 0;
                pp->current = 0;
                pp->once = 0;
            }
        }
        Sleep(pp->delay);
    }

    pp->exit = 0;
    pp->running = 0;
    return 0;
}

void set_console_title(string s)
{
    SetConsoleTitleA(s.c_str());
}

string directory()
{
    char Buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, Buffer);
    return Buffer;
}

void md_if(string s)
{
    system(("if not exist \"" + s + "\" md \"" + s + "\"").c_str());
}

int arg_find(string str, int k = 1)
{
    string s;
    if (k == 0)
        k = 1;
    for (int i = k; i < argcg; i++)
    {
        s = argvg[i];
        if (s == str)
            return i;
    }
    return 0;
}

string arg_value(string str, string *st = NULL, int k = 1)
{
    string s;
    for (int i = k; i < argcg - 1; i++)
    {
        s = argvg[i];
        if (s == str)
        {
            s = argvg[i + 1];
            if (st)
                *st = s;
            return s;
        }
    }
    return "";
}

string get_first_line(string file)
{
    string s;
    ifstream text(file);
    if (!text)
        return "";
    getline(text, s);
    text.close();
    return s;
}
string get_last_line(string file)
{
    string s;
    ifstream text(file,ios::ate);
    if(!text)   return "";

    long long i = text.tellg();

    text.seekg(i-1);
    char c;
    while(i)
    {
        text.get(c);
        if(c=='\n') break;
        i--;
        text.seekg(i);
    }
    getline(text,s);
    text.close();

    return s;
}

int spy_token_expired(string str)
{
    time_t curr_time, last_time;
    time(&curr_time);
    if (is_int(get_column(str, 2)))
    {
        last_time = (time_t)atoll(get_column(str, 2).c_str());
        if (curr_time - last_time > 3000)
            return 1;
        else
            return 0;
    }
    return 1;
}

string base64_encode(string in)
{
    string out;

    int val = 0, valb = -6;
    for (u_char c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

void exiting(string msg="") {
    if(msg != "") cout<<endl<<msg<<endl;
    if(argcg==1) {
        cout<<"\n\tPress Enter to exit...";
        string s;
        getline(cin, s);
    }
    exit(0);
}

string get_spy_token(string client)
{
    int retval;
    string token = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30 -X \"POST\" -H \"Authorization: Basic " + client + "\" -d grant_type=client_credentials https://accounts.spotify.com/api/token", &retval);
    if (retval)
        return "";
    Json::Value r;
    string errors;
    str2json(token, r, &errors);
    if (!r.isMember("access_token")) {
        throw string("UnableToGetToken");
    }

    return r["access_token"].asString();
}

bool del_file(string s, HANDLE h = NULL)
{
    if (h)
        CloseHandle(h);
    return DeleteFileA(s.c_str());
}

// expired token error response
/*
{
  "error": {
    "status": 401,
    "message": "The access token expired"
  }
}
*/


void token_client_saver(string token_file, string client_file, string token, string client) {
    if(get_last_line(token_file)!=token)
        write_to_text(token_file, token);
    if(get_last_line(client_file)!=client)
        write_to_text(client_file, client);
    pipe_open("attrib +h +s \"" + token_file+"\"");
    pipe_open("attrib +h +s \"" + client_file+"\"");
}


string generate_and_save_spy_token(string token_file, string client_file, string client="", bool save_token_client=true)
{
    if(client=="") client = get_last_line(client_file);
    if (str_find(client, ":"))
        client = base64_encode(client);

    string token = get_spy_token(client);
    if (save_token_client) token_client_saver(token_file, client_file, token, client);
    return token;
}

int spy_pl2tags(string pid, vector<spy_tag__> &v, string at, string &pln, string pln_user = "")
{
    string s, errors;
    Json::Value r;

    if (!str_find(pid, "spotify.com/playlist/") && !str_find(pid, "spotify:playlist:"))
        return 0;
    pid = replace_string_all(pid, "spotify:playlist:", "/playlist/");
    size_t f = str_find_index(pid, "/playlist/");
    if (f == -1)
        return 0;

    pid.erase(0, f + 10);
    pid = erase_find_found_to_end(pid, "?");
    int retval;
    string js_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30  -X \"GET\" \"https://api.spotify.com/v1/playlists/" + pid + "?fields=name\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer " + at + "\"", &retval);
    
    str2json(js_str, r, &errors);
    if(r["error"]["message"]=="The access token expired") throw string("AccessTokenExpired");

    if (retval)
        return 0;

    if (r.isMember("name"))
    {
        pln = r["name"].asString();
        pln = replace_string_all(pln, "\\", "_");
        pln = validate_path(pln);
    }
    else
        return 0;

    int js = 0, offset = 0;
    int no_artists = 0;
    spy_tag__ tag;
    pln_user = validate_path(pln_user);
    string plurl = "https://open.spotify.com/playlist/" + pid;
    while (1)
    {
        js_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30  -X \"GET\" \"https://api.spotify.com/v1/playlists/" + pid + "/tracks?fields=next%2Citems(track(album(artists%2Ctotal_tracks%2Cname%2Cid%2Cimages%2Crelease_date)%2Cname%2Cid%2Cartists%2Cduration_ms%2Cexternal_ids%2Ctrack_number))&offset=" + to_string(offset) + "\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer " + at + "\"", &retval);

        str2json(js_str, r, &errors);
        if(r["error"]["message"]=="The access token expired") throw string("AccessTokenExpired");
        if (retval)
            break;


        if (!r.isMember("items"))
            break;

        js = r["items"].size();

        for (int i = 0; i < js; i++)
        {
            tag.album = r["items"][i]["track"]["album"]["name"].asString();
            tag.album_url = "https://open.spotify.com/album/" + r["items"][i]["track"]["album"]["id"].asString();
            tag.cover = r["items"][i]["track"]["album"]["images"][0]["url"].asString();
            tag.date = r["items"][i]["track"]["album"]["release_date"].asString();

            tag.title = r["items"][i]["track"]["name"].asString();
            tag.url = "https://open.spotify.com/track/" + r["items"][i]["track"]["id"].asString();
            tag.isrc = r["items"][i]["track"]["external_ids"]["isrc"].asString();
            tag.tn = r["items"][i]["track"]["track_number"].asInt();
            tag.nos = r["items"][i]["track"]["album"]["total_tracks"].asInt();

            if (is_int(r["items"][i]["track"]["duration_ms"].asString()))
                tag.duration = r["items"][i]["track"]["duration_ms"].asString();
            else
                tag.duration = "0";

            if (pln_user != "")
                tag.playlist = pln_user;
            else
                tag.playlist = pln;

            tag.playlist_url = plurl;

            tag.sn = i + 1;

            no_artists = r["items"][i]["track"]["artists"].size();
            tag.artists = "";

            for (int j = 0; j < no_artists; j++)
            {
                if (j)
                    tag.artists += ",";
                tag.artists += r["items"][i]["track"]["artists"][j]["name"].asString();
            }

            no_artists = r["items"][i]["track"]["album"]["artists"].size();
            tag.albumartists = "";

            for (int j = 0; j < no_artists; j++)
            {
                if (j)
                    tag.albumartists += ",";
                tag.albumartists += r["items"][i]["track"]["album"]["artists"][j]["name"].asString();
            }

            v.push_back(tag);
        }

        offset += 100;

        if (r["next"].asString() == "")
            break;
    }

    return 1;
}

void text_to_vector(string file, vector<string> &v, int column = 0, int skip_empty = 1, int skip_first = 0)
{

    if (!exists(file))
        return;

    ifstream text(file);

    string s;
    if (skip_first && !text.eof())
        getline(text, s);

    while (!text.eof())
    {
        getline(text, s);

        if (s.length() == 0 && skip_empty == 1)
            continue;
        if (column)
            v.push_back(get_column(s, column));
        else
            v.push_back(s);
    }
    text.close();

    return;
}

int binary_search_string_vec(vector<string> &arr, int start, int end, string x, int col = 0)
{
    int mid;
    if (start == end)
        return -1;
    end--;
    while (start <= end)
    {
        mid = (start + end) / 2;

        if (!col)
        {
            if (arr[mid] == x)
                return mid;
            else if (arr[mid] < x)
                start = mid + 1;
            else
                end = mid - 1;
        }
        else
        {
            if (get_column(arr[mid], col) == x)
                return mid;
            else if (get_column(arr[mid], col) < x)
                start = mid + 1;
            else
                end = mid - 1;
        }
    }
    return -1;
}

Json::Value spy_get_album_json(string aid, string token, string country = "IN")
{
    string s, errors;
    Json::Value ra, rat, rn;

    if (!str_find(aid, "spotify.com/album/") && !str_find(aid, "spotify:album:"))
        return rn;
    aid = replace_string_all(aid, "spotify:album:", "/album/");
    size_t f = str_find_index(aid, "/album/");
    if (f == -1)
        return rn;

    string market;
    if (country != "")
        market = "&market=" + country;

    aid.erase(0, f + 7);
    aid = erase_find_found_to_end(aid, "?");
    int retval;
    string js_str;

    int js = 0, offset = 0;
    int no_artists = 0;
    spy_tag__ tag;
    string market_ = "?" + market;
    int ff = 0;

    js_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30 -X \"GET\" \"https://api.spotify.com/v1/albums/" + aid + market_ + "\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer " + token + "\"", &retval);
    str2json(js_str, ra);
    if(ra["error"]["message"]=="The access token expired") throw string("AccessTokenExpired");

    if (retval)
        return rn;
    if (!ra.isMember("tracks"))
        return rn;

    int nos = ra["tracks"]["total"].asInt();
    if (nos <= 50)
        offset = 51;

    while (offset < nos)
    {
        offset += 50;
        js_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30 -X \"GET\" \"https://api.spotify.com/v1/albums/" + aid + "/tracks?limit=50&offset=" + to_string(offset) + market + "\" -H \"Accept: application/json\" -H \"Content-Type: application/json\" -H \"Authorization: Bearer " + token + "\"", &retval);

        str2json(js_str, rat, &errors);
        if(rat["error"]["message"]=="The access token expired") throw string("AccessTokenExpired");
        if (retval)
            return rn;

        if (!rat.isMember("items"))
            return rn;

        js = rat["items"].size();

        for (int i = 0; i < js; i++)
            ra["tracks"]["items"].append(rat["items"][i]);
    }

    return ra;
}

int valid_album_title(string s)
{
    string fr = " - From \"";
    int f = str_find_index(s, fr);

    if (f == -1)
    {
        fr = " - From '";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " (From \"";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " (From '";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " [From \"";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " [From '";
        f = str_find_index(s, fr);
    }

    if (f != -1)
    {
        if (str_find(fr, "\"") && str_find_count(s, "\"", f + fr.length()) > 1)
            return 0;
        else if (str_find_count(s, "'", f + fr.length()) > 1)
            return 0;

        return 1;
    }

    return 0;
}

string extract_album(string s)
{
    string fr = " - From \"";
    int f = str_find_index(s, fr);

    if (f == -1)
    {
        fr = " - From '";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " (From \"";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " (From '";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " [From \"";
        f = str_find_index(s, fr);
    }
    if (f == -1)
    {
        fr = " [From '";
        f = str_find_index(s, fr);
    }
    if (f == lpn)
        return s;

    int g;
    if (str_find(fr, "\""))
        g = str_find_index(s, "\"", s.length() - 1, 1);
    else
        g = str_find_index(s, "'", s.length() - 1, 1);

    f += fr.length();
    string rs;
    for (int i = f; i < g; i++)
        rs.push_back(s[i]);
    return rs;
}

string extract_title(string s)
{
    string fr = " - From \"";
    int f = str_find_index(s,fr);

    if(f==-1) { fr = " - From '" ;   f = str_find_index(s,fr); }
    if(f==-1) { fr = " (From \"" ;   f = str_find_index(s,fr); }
    if(f==-1) { fr = " (From '"  ;   f = str_find_index(s,fr); }
    if(f==-1) { fr = " [From \"" ;   f = str_find_index(s,fr); }
    if(f==-1) { fr = " [From '"  ;   f = str_find_index(s,fr); }

    if(f!=lpn) s.erase(f,s.length()-f);
    return s;
}

string abs_path(string s)
{
    char buffer[4096];
    GetFullPathNameA(s.c_str(), 4096, buffer, NULL);
    return buffer;
}

string time()
{
    time_t timetoday;
    time(&timetoday);
    return replace_string_all(asctime(localtime(&timetoday)), "\n", "");
}

size_t file_size(string file)
{
    fstream fs(file, fstream::in | fstream::binary);
    fs.seekg(0, ios::end);

    size_t size = fs.tellg();
    if (size == lpn)
        size = 0;
    return size;
}

fstream append_file(string file, int create_new = 0)
{
    if (!file_size(file) || create_new)
    {
        ofstream temp(file);
        temp.close();

        fstream text(file, fstream::out);
        return text;
    }

    fstream text(file, fstream::in | fstream::out);

    text.seekg(-1, fstream::end);
    char c = text.get();
    if (c != '\n' && c != '\r')
        text << "\n";
    return text;
}

inline bool network_connection(int print = 0)
{
    return InternetCheckConnectionA("https://one.one.one.one", FLAG_ICC_FORCE_CONNECTION, 0);
}

void separate_columns(string s, vector<string> &v, char c='|')
{
    if(s=="")   return;
    stringstream ss = stringstream(s);

    while(!ss.eof())
    {
        getline(ss,s,'|');
        v.push_back(s);
    }
}

void str2argv(string s, vector<string> &v)
{
    int i=0, len, f;

    string cm, q, t;
    cm.push_back((char)CHAR_MAX);
    q.push_back((char)(CHAR_MAX-1));
    char c=' ';
//cm = "|";    
    while(s[0]==' ')    s.erase(0,1);
    while(s[s.length()-1]==' ')    s.erase(s.length()-1,1);

    s = replace_string_all(s,"\\\"",q);

    if(!str_find(s,"\""))
    {
        f = str_find_index(s,"  ");
        while(f!=-1)
        {
            s.replace(f,2," ");
            f = str_find_index(s,"  ");
        }
        if(s=="" || s==" ") return ;
        s = replace_string_all(s," ",cm);
    }
    else
    {
        s = " " + s;
        len = s.length();

        for(i=0; i<len; i++)
        {
            if(s[i]=='"' && c=='"')         c = ' ';    
            else if(s[i]=='"')              c = '"';   
            else if(s[i]==' ' && c=='"')    t += " ";
            else if(s[i]==' ')
            {
                t += cm;
                while(i<len-1 && s[i+1]==' ') i++;
            }
            else t += s[i];
        }
        t.erase(0,1);
        s = t;
    }
    
    s = replace_string_all(s,q,"\"");

    stringstream ss = stringstream(s);
    while(!ss.eof())
    {
        getline(ss,s,(char)CHAR_MAX);
        v.push_back(s);
    }
}

string vec_arg_value(vector<string> &v, string s, string *str=NULL)
{
    int argc = v.size();
    for(int i=0; i<argc-1; i++)
        if(v[i]==s)
        {
            if(str) *str = v[i+1];
            return v[i+1];
        }

    if(str) *str = "";
    return "";
}

struct js_search_tag__
{
    string title;
    string url;
    string duration;

    string get(string sep="|", string print="")
    {
        string s = title+sep+duration+sep+url;
        if(print!="") cout<<s<<print;
        return s;
    }
};

struct js_tag__
{
    string cover;
    string album;
    string album_url;
    string get(string sep="|", string print="")
    {
        string s = album+sep+album_url+sep+cover;
        if(print!="") cout<<s<<print;
        return s;
    }
};


string decode_html_chars(string s, int encode=0)
{
    if(!encode)   s = replace_string_all(s,"&quot;","\"");
    else          s = replace_string_all(s,"&quot;","\\\"");
    s = replace_string_all(s,"&amp;","&");
    s = replace_string_all(s,"&apos;","'");
    s = replace_string_all(s,"&lt;","<");
    s = replace_string_all(s,"&rt;",">");
    s = replace_string_all(s,"&#039;","'");

    return s;
}

DWORD shell_execute(string process, string param="", string dir="", int wait=1, int show=0)
{
    if(dir=="") dir = ".";
    SHELLEXECUTEINFOA info;

    info.cbSize = sizeof(SHELLEXECUTEINFO);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd = NULL;
    info.lpVerb = "open";
    info.lpFile = process.c_str();
    info.lpParameters = param.c_str();
    info.lpDirectory = dir.c_str();
    info.hInstApp = NULL;

    if(!show) info.nShow = SW_HIDE;
    else      info.nShow = SW_SHOWDEFAULT;
    
    DWORD pid = ShellExecuteExA(&info);
    if(!pid) return -1;

    if(wait && info.hProcess !=NULL)
    {
        ::WaitForSingleObject(info.hProcess, INFINITE);
        ::CloseHandle(info.hProcess);
    }

    pid = GetProcessId(info.hProcess);

    return pid;
}

int download_or_read_json(string link, string file, Json::Value &r, string member="")
{
    string json_str = text_to_string(file);
    string errors;
    int b = str2json(json_str,r);
    if(!b || (!r.isMember(member) && member!=""))
    {   
        int retval;
        json_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30 \""+link+"\"",&retval);
        if(retval) return 0;
        b = str2json(json_str,r);
        if(!b || (!r.isMember(member) && member!="")) return 0;
        write_to_text(file,json_str,0);
    }

    return b;
}



string get_album_id(string album_url)
{
    string json_str;
    int retval;
    json_str = pipe_open("C:\\Windows\\System32\\curl.exe --silent --max-time 30 \""+album_url+"\"",&retval);
    if(retval) return "";
    string pat = "\"params\":{\"albumid\":\"";
    size_t f = json_str.find(pat);
    if(f==lpn) return "";
    string album_id = copy_str(json_str, f+pat.length(), json_str.find("\"",f+pat.length()) - 1);
    return album_id;
}

void js_ate(string &str, vector<js_tag__> &v)
{
    size_t f = str_find_index(str,"href=\"/album/");
    size_t last = str_find_index(str,"</span></p></div></article></div></li></ol>");
    if(last==lpn) return ;

    js_tag__ t;
    string s, album_url;

    while(f<last && f!=lpn)
    {
        f = str_find_index(str,"title=\"",f,1);
        f += 7;

        s="";
        while(str[f]!='"')
        {
            s.push_back(str[f]);
            f++;
        }

        s = decode_html_chars(s);
        t.album = s;

        s="";

        f = str_find_index(str,"/album/",f);

        while(str[f]!='"')
        {
            s.push_back(str[f]);
            f++;
        }
        if(album_url==s)
        {
            f = str_find_index(str,"href=\"/album/",f+1);
            continue;
        }
        album_url = s;
        s = decode_html_chars(s);
        t.album_url = "https://www.jiosaavn.com"+s;

        v.push_back(t);
        //f = str_find_index(str,"href=\"/album/",f);
        f = str_find_index(str,"href=\"/album/",f+1);
    }
}

string url_encoder(string url) {
    ostringstream ostrm;
    ostrm.fill('0');
    ostrm << hex;
    vector<char> v = {'-', '_', '.', '~'};

    for (auto & c : url) {
        bool b = isalnum(c);
        if(!b) {
            for(auto & ch : v) {
                if(c==ch) {
                    b = true;
                    break;
                }
            }
        }
        if (b) {
            ostrm << c;
            continue;
        }
        ostrm << '%' << setw(2) << int((unsigned char) c);
    }

    return ostrm.str();
}


#endif