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

* [valgrind] - valgrind (manual: TERMINAL).

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



```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
