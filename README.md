Multi Language MCMC Speed Test
==============================

Introduction
------------

Useful in Bayesian Inference, [Markov Chain Monte Carlo](https://en.wikipedia.org/wiki/Markov_chain_Monte_Carlo) (MCMC) attempts to intelligently walk through parameter space to build posterior distributions.
MCMC accomplishes this by creating a numerical chain of accepted randomly drawn numbers, each link dependent on the previous step.

The image below shows a 5000 element trace of this problem.
The top left pane shows the parameter walk through the steps and the top right pane is data with fitted line.
The two bottom panes show the posterior distributions for the fit parameters (slope and intercept) with the 95% credible interval on either side of the median value.

![Sample 5000 element trace](mcmc_sample.png)

Why?
----

This started out as a pretty straight forward speed comparison between three languages: [Python](https://www.python.org/), [Go](https://golang.org/) and [C++](https://en.wikipedia.org/wiki/C%2B%2B).
The original attempt was to test the writability and performance for a task that was procedurally cpu intensive but also more exciting that coding up the Fibonacci sequence.
It has since turned into this MCMC rosetta stone for languages I just want to try out and compare against one another.
Natrually, each execution in any language yields a different number of total iterations I'm ultimately comparing the averaged per single iteration timespan.

Rules
-----

I write simple MCMC program using a plain Metropolis-Hastings Algorithm with a standard Gibbs sampler.
I really try to use only the standard library for each langauge while taking advantage of any language specific features that might make my life easier.
The task attempts to fit a line (y = ax + b) to a data set (x, y, dy) by creating a 500000 element trace for each parameter (a, b).
I've massaged the data and starting distributions enough to get a ~25% acceptance ratio which means each program will need to do ~2 million iterations.

Results
-------

| Language | Version | Total Iterations | Acceptance Ratio | Total CPU time (s) | Time/Iteration (ms) |
|:---:|:---:|:---:|:---:|:---:|:---:|
| Go | 1.4.2 | 1911726 | 26.2% | 2.3 | 1.2 |
| C++ | GCC 4.9.2 | 1841681 | 27.2% | 2.4 | 1.3 |
| PyPy | 2.5.1 | 1912917 | 26.1% | 3.9 | 2.0 |
| Python | 3.5.1 | 1914281 | 26.1% | 28.0 | 14.6 |
| Julia | 0.4.X | - | - | - | - |

Languages
---------

* [Julia](http://julialang.org/)
* [Python](https://www.python.org/)
* [PyPy](http://pypy.org/)
* [Go](https://golang.org/)
* [C++](https://en.wikipedia.org/wiki/C%2B%2B)
* [Fortran](https://en.wikipedia.org/wiki/Fortran)
