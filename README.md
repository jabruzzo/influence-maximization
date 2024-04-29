# The Influence Maximization Problem

## Background

Kempe, Kleinberg, and Tardos studied the Influence Maximization Problem in their 2003 paper, "Maximizing the Spread of Influence through a Social Network" (Kempe, Kleinberg, and Tardos, 2003). In the original version of the problem, we are given a directed social network and a model of behavior diffusion. Under the given model, we are able to estimate the expected number of individuals in the network who are "influenced" by a "seed set" of individuals. That is, we are able to estimate the total number of individuals who are expected to adopt the behavior at the end of the diffusion process given the seed set who adopted the behavior at the beginning. Our task is to find a seed set of $k$ individuals such that the expected number of individuals influenced by the seed set is maximized.

Kempe et al. (2003) and Kempe et al. (2015) showed that the Influence Maximization problem is NP-hard, but a simple greedy algorithm can find a good approximate solution. More specifically, the authors showed that because the objective function for the Influence Maximization Problem is non-negative, monotone non-decreasing, and submodular, their simple greedy algorithm returns a $(1-1/e-\epsilon)$-approximate solution to the problem. $\epsilon$ is an error term that arises because values of the objective function must be estimated rather than evaluated exactly (Kempe et al., 2015).

The greedy algorithm runs as follows. At the beginning of the algorithm, we start with an empty seed set $S_0 = \emptyset$. For $k$ iterations, we do the following: select the node that maximizes the estimated change in the objective function $f$ given the seed set assembled so far and add it to the seed set. That is, at iteration $i$, we add the node in the network $u$ that maximizes $$f(S_i \cup \lbrace u \rbrace) - f(S_i).$$ In other words, we choose the node that "adds the most influence" given the set of nodes already selected. After doing this for $k$ iterations, we end up with our desired approximately optimal seed set $S_k$.

We can define another version of the Influence Maximization Problem: the "sampled version" referenced by Wu and Küçükyavuz (2018, p. 579). Suppose we are given a set of $N$ influence cascades $\lbrace C_1, C_2, ..., C_N \rbrace$. These cascades are represented as directed acyclic graphs. The nodes of these graphs correspond to a set of individuals in a social network. The edges of these graphs represent influence. For example, if there is an edge pointing from node $u$ to node $v$ in cascade $C_a$, then node $u$ influenced node $v$'s behavior in cascade $C_a$. These cascades can either come from a behavior diffusion model (as in Wu and Küçükyavuz, 2018), or they can be generated from empirical data (as in Leskovec et al., 2007). As an example, the cascades could represent the sharing of news articles on social media. In this example, an edge from node $u$ to node $v$ in cascade $C_a$ means that node $v$ follows node $u$ and shared the link to article $a$ that node $u$ previously shared. In this version of the Influence Maximization Problem, we say that the influence of node $u$ in cascade $C_a$ is the number of nodes reachable from $u$ in $C_a$. Our task is to select the $k$ nodes whose average influence across the $N$ cascades is maximized. Kempe et al.'s greedy algorithm provides a $(1-1/e)$-approximate solution to this version of the problem (Wu and Küçükyavuz, 2018).

## This Repository

The purpose of the program in this repository is to implement the greedy algorithm of Kempe et al. (2003) in the "sampled version" of the Influence Maximization Problem described above. The program requires a parameter $k$, the desired size of the seed set, and a directory where the the set cascades are stored as edgelists in user-supplied `.txt` files. The program reads the information in the cascade files and then outputs an approximately optimal seed set according to the greedy algorithm of Kempe et al. (2003).

### Formatting the `.txt` Files

The directed acyclic graphs representing the cascades should be supplied by the user as edgelists in `.txt` files. Each directed edge should appear on its own line of the file. Nodes should be represented as positive integers, and there should be a space between the integers representing the nodes. The following shows the contents of an appropriately formatted cascade `.txt` file:  
```
1 2
2 3
2 4
```
The program does not check that the files are formatted correctly, and it does not check that the edgelists in the files represent directed acyclic graphs.

### Running the Code

1. Download the files in the repository.
2. Modify line 37 of `influence_maximization.cpp` to be the desired size of the seed set.
3. Modify line 40 of `influence_maximization.cpp` to be the directory where the cascade files are stored.
4. If you compile and execute the program using the sample cascades included in the repository with the seed set size set to 1, the following should print to the console:
   ```
   READING CASCADES...

   CASCADES READ! NUMBER OF CASCADES: 4

   RUNNING GREEDY ALGORITHM...

   GREEDY ALGORITHM FINISHED!

   APPROXIMATELY OPTIMAL SET (SIZE 1): {1}

   INFLUENCE OF APPROX. OPTIMAL SET (NUMBER OF NODES): 2.000000

   TIME (SEC): 0
   ```

## References

Kempe, D., Kleinberg, J., & Tardos, É. (2003, August). Maximizing the spread of influence through a social network. In _Proceedings of the ninth ACM SIGKDD international conference on Knowledge discovery and data mining_ (pp. 137-146).

Kempe, D., Kleinberg, J., & Tardos, É. (2015). Maximizing the Spread of Influence through a Social Network. _Theory of Computing, 11_(4), 105-147.

Leskovec, J., Krause, A., Guestrin, C., Faloutsos, C., VanBriesen, J., & Glance, N. (2007, August). Cost-effective outbreak detection in networks. In _Proceedings of the 13th ACM SIGKDD international conference on Knowledge discovery and data mining_ (pp. 420-429).

Wu, H. H., & Küçükyavuz, S. (2018). A two-stage stochastic programming approach for influence maximization in social networks. _Computational Optimization and Applications, 69_, 563-595.
