Go/C++/Python MCMC Speed Test
=============================

Introduction
------------

I've been meaning to learn [Go](http://golang.org/) for a while and decided to take the plunge after speaking with a professional data analyst.
If Go was going to fit into my life it would have to fulfill a certain role in my computational needs.
I primarily use Python for many tasks because it's just lovely to work with.
It is however dreadfully slow on computationally intensive tasks if you can't figure out how to reformulate your recipe to take advantage of speed gains in numpy, as one example.
For speed and general low level number crunching I had been using C++ but wasn't enjoying it.
I found the ease and rhythm of writing code in Python railroaded by C++.

Perhaps Go could replace C++ for this purpose so I decided to attempt a speed test using a task I would routinely leave Python for in favour of C++, [Markov Chain Monte Carlo](https://en.wikipedia.org/wiki/Markov_chain_Monte_Carlo) (MCMC).
Useful in Bayesian Inference, MCMC attempts to intelligently walk through parameter space to build probability distributions of the solution.
MCMC accomplishes this by creating a numerical chain of accepted randomly drawn numbers, each link dependent on the previous step.

The image below shows a 5000 element trace of this problem.
The top left pane shows the parameter walk through the steps and the top right pane is data with fitted line.
The two bottom panes show the probability distributions for the parameters with the 95% credible interval on either side of the median value.

![Sample 5000 element trace](mcmc_sample.png)

I wrote a very simple MCMC program in Go, C++ and Python making sure to construct each in a similar fashion while taking advantage of any language specific features that might make my life easier.
I'm using mostly standard libraries (and numpy in Python).
Each program attempts to fit a line (y = ax + b) to a data set (x, y, dy) by creating a 500000 element trace for each parameter (a, b).
I've massaged the data and starting distributions enough to get a ~25% acceptance ratio which means each program will need to do ~2 million iterations.

Let's address the elephant in the room, yes I am comparing the execution speeds of programs that aren't doing _exactly_ the same number of iterations.
They are however, preforming the same task which I, and possibly others, would find more useful than simply calculating a Fibonacci sequence.
I'm also including a time per iteration in the analysis which will normalize any inconsistencies in the total number of steps.

Results
-------

| Language | Version | Total Iterations | Acceptance Ratio | Total CPU time (s) | Time/Iteration (ms) |
|:---:|:---:|:---:|:---:|:---:|:---:|
| Go | 1.4.2 | 1911726 | 26.2% | 2.3 | 1.2 |
| C++ | GCC 4.9.2 | 1841681 | 27.2% | 2.4 | 1.3 |
| Python | 2.7.9 | 1907274 | 26.2% | 27.7 | 14.5 |
| Python | 3.4.3 | 1910038 | 26.2% | 27.3 | 14.3 |

Not only is Go as fast as C++ but it's quite nice to code in.
Go has the readability and ease of python without the overhead of C++.
It's also worth noting that I compiled the C++ code with the '-O2' optimizer flag which cut the run time in half.
If you're at all interested check out the [official go lang book](http://www.golang-book.com).

__Thank you__ to [Andrew Valencik](https://github.com/valencik) for exposing my over reliance on the numpy library in Python, even if he was kind of a dick about it. He managed to get a factor of 20 speed up from the Python mcmc implementation by using vanilla Python arrays over numpy.
