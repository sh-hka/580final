# cpp-final-project-abs

## Project Description 

Everyone in the class is expected work on a project and present it - this is your final exam! You can work individually or in groups of up to 4 students (zero exceptions on group size!).

If you are working in a group, you must let me know the group as soon as possible.

### Grading

#### Core Requirements

* Code is submitted through GitHub
* A written report is required
  * This should be about a page, and should be provided as a file named README.md with your code. Format your README.md nicely using GitHub Markdown!
  * This report should detail the project, challenges encountered, results, and possible improvements.
* Your project shall compile and execute on turtleshell/clamshell!

#### Breakdown

| Scoring Criteria | 4 | 3 | 2 | 1 | 0 | Weight |
| ---------------- | --- | --- | --- | --- | --- | --- |
| Compilation      | Compiles | N/A | N/A | N/A | Does not compile | 6 |
| Execution        | No Errors | Passes Most Tests | Executes, Does Not Work | Does Not Execute | Does Not Execute | 13 |
| Submission       | Submitted on time to GitHub | N/A | N/A | Submitted to GitHub | No Submission | 2 |
| Presentation     | Report is complete and presented | N/A | ** | N/A | Missing Report or Presentation | 4 |

\*\* Report and presentation are given; not only are key details missing, but project understanding is lacking

## Academic Integrity

Many implementations already exist freely on the internet; you may look at them, but you must cite them as sources, **failure to do so is plagiarism**.

## Abelian Sandpile Model

### The Algorithm

The model's algorithm is very simple. Given some 2 dimensional grid (lattice, matrix, etc.) of integers, repeatedly "topple" individual cells that are too high until every cell is low enough. The toppling of a cell occurs when its value is in a critical state, that is, when its value is greater than 3. When a cell topples, its value is reduced by 4 and the values of its nearest neighbors are increased by by 1.

Here is psuedocode:

```text
1) Let P = MxN matrix of positive integers
2) While there exists x in P such that x >= 4 (i.e. x is critical):
    A) for each element e of P:
        a) if e >= 4
            i)   subtract 4 from e
            ii)  add 1 to the neighbor north of e **
            iii) add 1 to the neighbor south of e **
            iv)  add 1 to the neighbor east of e  **
            v)   add 1 to the neighbor west of e  **

            ** if there is no neighbor in that direction, do nothing
```

For example, if we have a 3x3 grid:

||||
|-|-|-|
| 0 | 3 | 0 |
| 3 | 4 | 3 |
| 0 | 3 | 0 |

The 4 in the center is *critical* and collapses. The new state of the grid becomes:

||||
|-|-|-|
| 0 | 4 | 0 |
| 4 | 0 | 4 |
| 0 | 4 | 0 |

As we can see, we now have 4 critical cells that all must be collapsed. The new state of the grid becomes:

||||
|-|-|-|
| 2 | 0 | 2 |
| 0 | 4 | 0 |
| 2 | 0 | 2 |

And finally...

||||
|-|-|-|
| 2 | 1 | 2 |
| 1 | 0 | 1 |
| 2 | 1 | 2 |

Now our sandpile has no critical cells and is *stable*. Your goal is to collect all of the intermediate states of the sandpile until you reach a stable state!

Information on the algorithm can be found [here](https://en.wikipedia.org/wiki/Abelian_sandpile_model).

### Functional Requirements

Your program should be able to run in two modes:

1) given a size (S) and a count (N), use a random number generator to drop N random grains onto an empty sandpile that has `S` rows and `S` columns
2) given a starting sand pile file, load the sandpile from the file.

In both modes you need to stablize the sandpile iteratively, and capture the sandpile after each sweep. This means that each time you complete step `2.A` in the pseudo code, we need to buffer it for visualization. In order to make our code a little simpler, we can use the following *type-aliases*:

#### Random Number Generation

You are incrementing random cells by 1, so this means you need to select a random coordinate. This is done by selecting two random numbers in the range `(0,S)`, where `S` is the size of your sandpile. The first random number you select is the row, and the second random number is the column. Use a 64-bit Mersenne Twister with a seed of `1337`, and a uniform integer distribution from 0 to `S`.

#### Reading From a File

You code will be expected to read a space delimited file given to you - you should use `std::ifstream` to open the file, and a while-loop with `std::getline` to read each line of the file. You should then use the following code for turning that line into a vector, and store it into our sandpile:

```cpp
std::istringstream ss(buffer);
std::copy(std::istream_iterator<int>(ss), {}, std::back_inserter(sandpile.emplace_back()));
```

This specific code snippet makes the following assumptions:

* You include the headers `algorithm`, `iterator`, `sstream`
* your line buffer for reading from the file is named `buffer`
* you have a 2D `std::vector` available named `sandpile`

A `stringstream` is a special string-wrapper class that allows the string to be passed along as a mutable stream, We use `istream_iterator` to allow the `std::copy` algorithm to treat the buffer like other C++ STL containers (this is similar to saying `std::begin( ... )`). The empty `{}` is effectively our `std::end` bit, and `std::back_inserter` is a special tool that allows us to insert into a row within the sandpile as the destination for the copy.

All in all, this code says to convert the buffer into a mutable stream, and then algorithmically copy each chunk of the stream (automically splitting on spaces) into a new row of the sandpile.

### Code Organization

Your code for generating the random sandpile and the code for loading a sandpile from a file should be contained in header and source files respectively named `make_sandpile.h` and `make_sandpile.cpp`. You should have a function to do each task, forward declared in the header file and defined completely in the source file.

Your code for processing a sandpile should be in header and source files respectively named `sandpile.h` and `sandpile.cpp`.

You can use the following type-aliases for simplifying code.

```cpp
using Row = std::vector<int>;
using Sandpile = std::vector<Row>;
using Animation = std::vector<Sandpile>;
```

This allows us to use the names `Row`, `Sandpile`, and `Animation` as nicknames for their respective data types!

### Visualizing

In order to visualize your sandpile, we will make use of `malen-bokeh`, provided under the directory `project/include/run-sci-comp/`. We also have a serializer we can use to save binary records of the entire sandpile.

## Using the Visualizer

The Abelian Sandpile Model is a system that progresses over time. Every iteration of the core algorithm should produce a *frame*. If we string all of the frames together we would get an animation that shows the progression of the system. Given that a frame is a snapshot of the sandpile, it has the type `std::vector<std::vector<int>>`. If we collect these frames in a vector we end up with data type `std::vector<std::vector<std::vector<int>>>` to represent all of the data for the complete animation... this is a lot of data!

The idea is that after each iteration you can push your entire sandpile into another vector. For simplicity, you can do the following in your code:



Then you can do:


And finally at the end of your program you can then do something like this:

```cpp
animate(animation);
serialize(animation);
```

The function `animate` expects your sandpile data to be of the type `Animation` (i.e. `std::vector<std::vector<std::vector<int>>>`). This code will create a graph that includes a slider and a play button so that you can observe how the sandpile changes over time. You can see an [example here](http://crab.rutgers.edu/~nesan/sandpile.html). This is the result of a sandpile that is 50x50 with each cell initialized to 4 (critical everywhere!). It takes nearly 400 iterations to stablize.

Note that the computational complexity of the sandpile increases exponentially with the size of the your sandpile; a grid that is 100x100 will take over 1000 iterations, which results in a large amount of data. This will result in the graph loading very slowly in your browser (if at all!); and so you should not work with any sandpile greater than 100x100.

