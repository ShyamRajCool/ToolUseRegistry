# Project 3 Design Log
**Author:** <Shyam Rajaram> · **P2 code used:** <my own>
**Tools used:** <Gemini (Pro) Understanding the specifications, namely understanding and differentiating between what was given it the specs vs what I needed to hunt for. I also used it to understand some of the test cases, like for each. It did not make sense though. I also debugged issues with my file structure with it. I also used Gemini, stack overflow and google to understand some of the syntax like string_view.>


<!-- 500–800 words. An argument, not a diary: say what you decided, what
     you rejected, and what evidence backs it up. Delete these comments
     before submitting. -->

## 1. Hash table
Chaining using the suggested std::vector was used in the Hash table to avoid collisions that would be a little difficult to think about when using 
linear probing. Std::vector simplifies this and a linked-list would make it is little bit more invovled. Std::vector is simple to use. The 
hash function was FNV-1a because the implementation was provided and I wanted to make sure what I implemented was both thorough and easy to implement.
The load-factor threshold is the tools to bucket ratio, and by keeping it within the 1.0 limit, O(1) search is done since a chain doesn't usually need to be parsed. The add usually has a spot to place it in because of this load factor criteria, so it can work seamlessly. The resize policy is doubling the bucket count.

bucket count: 16384
Load factor: 0.610352
Max proble length: 5
Longest Chain: 9000


## 2. Pointer stability
My vector_elements in the find() function are of the type unique_ptr so they need to be moved. It is move semantics meaning that nothing is destroyed, just stolen. Even if the original vector is resized, it is imperative that the unique_ptrs are preserved to make the recreation process simple as well. Therefore, Tool* returned by find() survives a resize in my design, in which the unique ptr is given to us in the .h file. If it wasn't then doing this project would be very hard.

## 3. Calculator trace

(Left to right throughout as a first principle)

2 is a number and is pushed to the values stack | Values[2] Ops[]
* is an operator and is pushed to the operator stack | Values[2] Ops[*]
( is an operator and is pushed to the operator stack | Values[2] Ops[(,*]
3 is a number and is pushed to the number stack | Values[3,2] Ops[(,*]
+ is an operator and is pushed to the operator stack | Values[3,2] Ops[+,(,*]
4 is a number and is pushed to the number stack | Values[4,3,2] Ops[+,(,*]
 ) is an operator and is found and the following statement applies:

 *Because of the closing parentheses, The values stack top two values 4 and 3 are popped. The closing parentheses isn't pushed. In addition, the top value from the Ops stack, +, is also popped. Result 7 is pushed to Values stack | Values[7,2] Ops[*]*

*Since * takes precedence over -, 2 and 7 are popped and evaulated. 14 is pushed to Values stack and - is pushed to Ops stack | Values[14] Ops[-]*

5 is a number and is pushed to the number stack | Values[5,14] Ops[-]

Evaluate - to pop 5 and 14 and pop - from the stacks to put 9 on Values stack | Values[9] Ops[]


## 4. Errors as values


     The errors returning are good because it could that a file doesn't exist or a path is unable to be accessed. Some other method can be tried. However, for anything that can cause actual harm like accessing out of bounds memory should be a case for an exception as that can cause unintended/unexpected return values.


## 5. What I would design differently

     The design works in the test cases, but I would try to incorporate more operators like arithmetic left/right shift. The workspace.cpp file is well optimized. I would also try the linear probing method but it is a little bit more complicated. However, in class the thought process would be having cache that is close to one another, so the spatial locality is good. The chaining could have poor locality. For this application, it is fine, however.