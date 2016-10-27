# Sudoku
A hard coded low level sudoku game written in c++ using [ncurses](https://en.wikipedia.org/wiki/Ncurses)

#### Prerequisite
[Install](http://www.cyberciti.biz/faq/linux-error-cursesh-no-such-file-directory/) ncurses.
###### RHEL / Fedora / CentOS Linux
```
  $ yum install ncurses-devel ncurses
```
###### Debian / Ubuntu Linux
```
  $ sudo apt-get install libncurses5-dev libncursesw5-dev
```

#### Compile
```
  $ g++ sudoku.cpp -o sudoku -lncurses
```

#### Run
```
  $ ./sudoku
```

#### Screenshots

![game](https://raw.githubusercontent.com/ahmed-dinar/sudoku-game/master/Screenshot.png)
![check](https://raw.githubusercontent.com/ahmed-dinar/sudoku-game/master/Screenshot2.png)

#### About
I made this game in 2014 to learn UI in c++.
