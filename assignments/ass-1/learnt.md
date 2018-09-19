## The C Language
There are things called lvalues and rvalues in C. 

### Locator Value
The locator value is an expression that locates a certain object. These expressions can be `[]`, `*`, a variable, a constant, pretty much anything that helps identify some object. Note the distinction from strictly meaning "memory address".

### R Value
The rvalue is an expression that cannot have a value assigned to it.

## Processes

### Memory Map
The sections are *code*, *read-only*, *global data*, *stack*, *kernel* and *heap*. The order is apparent from the below text. Here is an image summarizing the map:

![](./mem.png)

#### Code (Text Segment)
The text segment has read and execute priviliges to the section. GCC Usually puts the code instructions and global constants here. There are ways of moving global constants to the *read-only* section, by configuring GCC.

#### Read Only
This section is above the text segment, and gives only read-priviliges. This is where global constants *would* go otherwise.

#### Global Data
Other global data is put in this section, which is above the read-only section. The Global Data section gives read and write priviliges.

#### 
