GUIDLINE TO CONTRIBUTING THE SDK

Developers must do things as described in this document.

**This document might be changed over the time**.

+ Line width

    The maximum line width for source files is 80 characters, whenever possible.
    
    Longer lines are usually an indication that you either need a function
or a pre-processor macro.

+ Indentation

    Each new level is indented 2 or more spaces than the previous level:

    ```
    if (condition)
        single_statement ();
    ```

    
    This can only be achieved using space characters. It may not be achieved using tab characters alone, or using a combination of spaces and tabs.
+ Tab characters

    The tab character must always be expanded to spaces. If a literal
tab must be used inside the source, the tab must always be interpreted
according to its traditional meaning:

	Advance to the next column which is a multiple of 8.
        [ these two lines should be aligned ]
+ Braces

    Curly braces should not be used for single statement blocks:
    ```
    if (condition)
        single_statement ();
    else
        another_single_statement (arg1);
    ```
    In case of multiple statements, curly braces should be put on another
    indentation level:
    ```
    if (condition)
        {
        statement_1 ();
        statement_2 ();
        statement_3 ();
        }
    ```
    The "no block for single statements" rule has only four exceptions:

    1.  if the single statement covers multiple lines, e.g. for functions with many arguments, and it is followed by else or else if:
    ```
    /* valid */
    if (condition)
        {
        a_single_statement_with_many_arguments (some_lengthy_argument,
                                                another_lengthy_argument,
                                                and_another_one,
                                                plus_one);
        }
    else
        another_single_statement (arg1, arg2);
    ```
    2.  if the condition is composed of many lines:
    ```
    /* valid */
    if (condition1 ||
        (condition2 && condition3) ||
        condition4 ||
        (condition5 && (condition6 || condition7)))
        {
        a_single_statement ();
        }
    ```
    3.  Nested if's, in which case the block should be placed on the
        outermost if:
    ```
    /* valid */
    if (condition)
        {
        if (another_condition)
            single_statement ();
        else
            another_single_statement ();
        }
    ```
    ```
    /* invalid */
    if (condition)
        if (another_condition)
        single_statement ();
        else if (yet_another_condition)
        another_single_statement ();
    ```
    4.  If either side of an if-else statement has braces, both sides
        should, to match up indentation:
    ```
    /* valid */
    if (condition)
        {
        foo ();
        bar ();
        }
    else
        {
        baz ();
        }
    ```
    ```
    /* invalid */
    if (condition)
        {
        foo ();
        bar ();
        }
    else
        baz ();
    ```

+ Function name

    We use snake case: using `internal_underscores_to_break_words`
