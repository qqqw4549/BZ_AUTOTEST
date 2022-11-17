#include <command.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#define COMMAND_MAX_SIZE 300
#define VARLISTMAX  10 
#define CMD_PROMPT_STR "BZRU->"



typedef int (*CommandHandler)(int,int,int,int,int,int,int,int,int,int);
typedef struct Command
{
    const char *cmd;
    const char *help;
    CommandHandler handler;
}Command;

static Command commands[COMMAND_MAX_SIZE] = { 0 };

static int FindCommand(char * str, Command ** pCmd)
{
    if(0 == str || 0 == pCmd)
        return 0;

    int i;
    for(i = 0; i < COMMAND_MAX_SIZE; i++)
    {
        if( 0 == commands[i].handler)
            break;

        int lenStr = strlen((const char *)str);
        int len = strlen(commands[i].cmd);

        if(0 == memcmp(commands[i].cmd, (const char *)str, (len > lenStr) ? len : lenStr))
        {
            *pCmd = &commands[i];
            return 1;
        }
    }
    return 0;
}

//int ExecuteCmd(cmd *pCmd, U32 pa1, U32 pa2, U32 pa3, U32 pa4, U32 pa5)
static int ExecuteCommand(Command *pCmd, 
    uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, 
    uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10)
{
    if(0 == pCmd || 0 == pCmd->handler)
    {
        printf("\r\nExecuteCmd para error");
        return 0;
    }
    return (pCmd->handler)(p1, p2,p3,p4,p5,p6,p7,p8,p9,p10);
}

static void ParseCommand(char *cmdString)
{
    Command *pCmd = 0;
    char *p = 0,*cmd = 0;
    uint32_t varList[VARLISTMAX] = { 0 };
    uint32_t i = 0;

    add_history (cmdString);
    if((('/' == cmdString[0]) && ('/' == cmdString[1]))
            || ('@' == cmdString[0])
            || ('#' == cmdString[0]))
    {
        printf("\r\nAnnotating code!");
        printf("\r\n"CMD_PROMPT_STR);
        fflush(stdout);
    }
    else
    {
        p = strtok((char *)cmdString," ");
        if(0 != p)
        {
            cmd = p;
            while(p)
            {
                p = strtok(0," ");
                if(0 == p)
                    break;
                if(i < VARLISTMAX)
                {
                    if ((('/' == p[0]) && ('/' ==p[1]))
                        ||('@' == p[0])
                        ||('#' == p[0]))
                    {
                        break;
                    }
                    if(p[0] == '0' && p[1] == 'x')
                    {
                        p += 2;
                        varList[i++] = strtoul(p,0,16);
                    }
                    else if(p[0] == 'c')
                    {
                        p++;
                        varList[i++] = atof(p);
                    }
                    else
                    {
                        varList[i++] = strtoul(p,0,10);
                    }
                }
                else
                {
                    break;
                }
            }
        }

        if(FindCommand(cmd, &pCmd))
            ExecuteCommand(pCmd, varList[0], varList[1], varList[2], varList[3], varList[4], varList[5], varList[6], varList[7], varList[8], varList[9]);
        else
            printf("\r\nUnknown command \r\n");
    }
}

int32_t RegisterCommand(const char* cmd, const char* help, void *handler)
{
    if(!cmd || !help || !handler)
    {
        printf("%s: invalid param\n", __func__);
        return 1;
    }

    uint32_t i;

    for(i = 0; i < COMMAND_MAX_SIZE; i++)
    {
        if(!commands[i].handler)
        {
            commands[i].cmd = cmd;
            commands[i].help = help;
            commands[i].handler = handler;
            break;
        }
    }

    if(i == COMMAND_MAX_SIZE)
    {
        printf("%s: is max size=%u\n", __func__, i);
        return 2;
    }

    return 0;
}

uint8_t runflag = 1;

static void ExitCommand(void)
{
    runflag = 0;
}


int32_t WaitForCommand(void)
{
    int value;
    uint8_t len = 0;
    uint8_t cursor = 0;
    RegisterCommand("help", "show this page", ShowCommands);
    RegisterCommand("exit", "exit the program", ExitCommand);
    rl_bind_key('\t', rl_insert);
    ShowCommands();
    while(runflag)
    {
        char *line_read = readline(CMD_PROMPT_STR);
        if(!line_read)
            continue;
        ParseCommand(line_read);
        free(line_read);
    }
    return 0;
}

void ShowCommands(void)
{
    uint32_t i;
    for(i = 0; i < COMMAND_MAX_SIZE; i++)
    {
        if(commands[i].handler)
            printf("%-10s: %s\n", commands[i].cmd, commands[i].help);
    }
}
