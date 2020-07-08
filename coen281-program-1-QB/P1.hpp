/*
 
 P1.hpp
 coen281-program-1-QB

 Created by Quan Bach  on 7/4/20.
 Copyright © 2020 QB. All rights reserved.
 
 Header file for P1.cpp
 
 Santa Clara University
 COEN 281 - Data Mining
 Program Assignment 1
  This program will opperate on multiple input documents and determine the simmilarities among them. The program will return a Jaccard similarity score between the documents to determine if any document is plagiarisez. The program implements shingling, minhashing, and locality-sensitive hashing.
 Complexity: O(N)
 */

#ifndef P1_h
#define P1_h

/* Include Files */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> /* remove; remove_if; replace_if; */
#include <iterator>
#include <unordered_set>
#include <set>
#include <iomanip>
#include <unordered_map>

/* Functions Prototypes */
int hash_shingle (std::string str);

std::vector<std::string> generate_shingles_vector (std::string &str);

bool isPrime (int n);

int LSH_hash_function (std::vector<int> vec);

#endif /* P1_h */
