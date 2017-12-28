# FootBall_data (ISEL PSC 1718i)

Programmed application in C language for the subject "Programming in Computational Systems - PSC", using a Linux VMachine.
This app queries a server, with jansson and curl libraries, where it finds data of several football competitions, teams and games.
* ISEL - Intituto Superior de Engenharia de Lisboa.
* LEIC - Licenciatura em Engenharia Informática e de Computadores.

## Getting Started

To get a copy of this project running on your local machine you need to follow these next steps...

### Prerequisites

You will need a Linux machine, physical or virtual, your choice.
This project in particular was made using Ubuntu distro.

### Installing

What you will need:

* [curl](http://curl.haxx.se/libcurl) - CURL library documentation.

```
sudo apt-get install libcurl4-gnutls-dev
```

* [jansson](https://jansson.readthedocs.io/en/2.7/index.html) - JSON library documentation.

```
sudo apt-get install libjansson-dev
```

* [valgrind](http://valgrind.org/) - valgrind (manual: TERMINAL).

```
sudo apt-get install valgrind
man valgrind
```

Now you are ready to compile and test the code.

## Running the application

To change the season from where the application will look for the competitions, in the application arguments write the year, if not, it will look for the current competitions.

```
./football_data {year}
./football_data
```

After compiling and with the application running, you can use the following commands:

List competitions:

```
c
```

List teams for a determinated competition:

```
t {competition_id}
```

List games for a determinated competition:

```
x {competition_id}
```

List games for a determinated team in all queried competitions till now:

```
j {team_id}
```

Close the app:

```
q
```

Any other unrecognized command will show all the available commands.

## How it works

When the app is running, it will wait for user input, showing the ">" char.

Lets say that we want to check the current competitions, we input "c" and it will give us a list.
Thats a linked list of struct 'Competition'.

Next we want to see which teams play in one of those competitions, example ID: 445.
Input will be "t 445", will then show a list with all the teams for that competition.
This list is a linked list of struct 'Team' inside a linked list of struct 'TeamNode'.
TeamNode is were we save all the queried teams till now, so later on we dont need to query the server again.

Now lets get all the games for competition, example 445.
Input will be "x 445", and show us a list of all the games for that competition as expected.
Its a linked list of struct 'Fixture' inside a linked list of struct 'FixtureNode'.
Again, FixtureNode because we dont want to query the server again later on, so we save the info in memory in a list.

How about he get all the games that Chelsea plays in all the competitions that we queried so far?
Input "j CFC", CFC is the team ID for Chelsea that we got from the "t 445" command.
And we got all the games printed in a list format from all the info we got before from the server.

```
c
t 445
x 445
j CFC
```

## Authors

* **Rodrigo Silva** - *ISEL A41429* - [PiniponSelvagem](https://github.com/PiniponSelvagem)
* **Marta Mendes** - *ISEL A42126*
* **David Soares** - *ISEL A42193*
