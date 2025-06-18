# Minishell
Project at 42 - Replica of a Shell terminal (not a full one, the goal is to learn the basic elements about Shell terminal)

## Evaluation
This project is far from being perfect, for instance there wee still some small "still reachables" within ou code, some redirections were misfunctionning and trimming was not perfect. However regarding the level of detail and precision required by Shell, it is already a satisfyingly functionnal project that is enoughh to validate.

Some advices for surviving the examination :
- Stay aware that the readline package and external commands may cause leaks ad still reachables. In tht case, think about adding a .supp file that clean these errors on Valgrind so that you can get away easily.
- Make sure that unsetting environment variables as $PATH doesnot cause any leak. We forgot it for our first try causing us to retry it.
- Make sure to use the global variable *only* for Signals. Exit status must be sorted as a separate variable that shouldn't be a global one (the best thing is to but it within a structure).
- Don't hesitate to check this [excel_file](https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit?gid=0#gid=0) containing 800+ tests, if you pass 75% of them, then you should be able to validate and have a project that is solid enough. There are also several cas figures that should help(some you never think about but that may be useful).
- Run this to check every leaks valgrind --leak-check=full --trace-children=yes --track-fds=yes --suppressions=readline.supp (optionnal if you have a .supp file) -q (only shows when there are big leaks) ./minishell

Good Luck !
