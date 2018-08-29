#include <string.h>
#include <stdio.h>

typedef struct item_t 
{
    char *key;
    char *value;
}ITEM;



char *strtrimr(char *pstr)
{
    int i;
    i = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i >= 0))
        pstr[i--] = '\0';
    return pstr;
}


char *strtriml(char *pstr)
{
    int i = 0,j;
    j = strlen(pstr) - 1;
    while (isspace(pstr[i]) && (i <= j))
        i++;
    if (0<i)
        strcpy(pstr, &pstr[i]);
    return pstr;
}


char *strtrim(char *pstr)
{
    char *p;
    p = strtrimr(pstr);
    return strtriml(p);
}


int get_item_from_line(char *line, ITEM *item)
{
    char *p = strtrim(line);
    int len = strlen(p);

    if(len <= 0)
	{
        return 1;
    }
    else if(p[0]=='#')
	{
        return 2;
    }
	else
	{
        char *p2 = strchr(p, '=');
        *p2++ = '\0';
        item->key = (char *)malloc(strlen(p) + 1);
        item->value = (char *)malloc(strlen(p2) + 1);
        strcpy(item->key,p);
        strcpy(item->value,p2);
	}
    return 0;
}

int file_to_items(const char *file, ITEM *items, int *num)
{
    char line[1024];
    FILE *fp;
    fp = fopen(file,"r");
    if(fp == NULL)
        return 1;
    int i = 0;

    while(fgets(line, 1023, fp))
	{
        char *p = strtrim(line);
        int len = strlen(p);
        if(len <= 0)
		{
            continue;
        }
        else if(p[0]=='#')
		{
            continue;
        }
		else
		{
            char *p2 = strchr(p, '=');
            if(p2 == NULL)
                continue;
            *p2++ = '\0';
            items[i].key = (char *)malloc(strlen(p) + 1);
            items[i].value = (char *)malloc(strlen(p2) + 1);
            strcpy(items[i].key,p);
            strcpy(items[i].value,p2);

            i++;
        }
    }
    (*num) = i;
    fclose(fp);
    return 0;
}



int read_conf_value(const char *key, char *value, const char *file)
{
    char line[1024];
    FILE *fp;
    fp = fopen(file,"r");
    if(fp == NULL)
	{
		printf("open config file error\r\n");
        return 1;
	}
    while (fgets(line, 1023, fp))
	{
        ITEM item;
        get_item_from_line(line, &item);
        if(!strcmp(item.key, key))
		{
            strcpy(value, item.value);

            fclose(fp);
            free(item.key);
            free(item.value);
            break;
        }
    }
    return 0;
}

int write_conf_value(const char *key, char *value, const char *file)
{
    ITEM items[20];
    int num;

    file_to_items(file, items, &num);

    int i=0;
   
    for(i=0;i<num;i++)
	{
        if(!strcmp(items[i].key, key))
		{
            items[i].value = value;
            break;
        }
    }


    FILE *fp;
    fp = fopen(file, "w");
    if(fp == NULL)
	{
        return 1;
	}

    i=0;
    for(i=0;i<num;i++)
	{
        fprintf(fp,"%s=%s\n",items[i].key, items[i].value);
    }
    fclose(fp);

/*
	i=0;
    for(i=0;i<num;i++){
        free(items[i].key);
        free(items[i].value);
    }
*/
    return 0;
}


unsigned int set_avi_filecount(unsigned int value)
{
	char val[32];
	sprintf(val, "%d", value);
	write_conf_value("AVICNT", val, "/dm368/filecnt.ini");
	printf("============AVICNT========%s\r\n", val);
}

unsigned int set_bmp_filecount(unsigned int value)
{
	char val[32];
	sprintf(val, "%d", value);
	write_conf_value("BMPCNT", val, "/dm368/filecnt.ini");
	printf("============BMPCNT========%s\r\n", val);
}

unsigned int get_bmp_filecount()
{
	char val[32];
	unsigned int ret;
	memset(val, 0, 32);
	read_conf_value("BMPCNT", val, "/dm368/filecnt.ini");
	ret = atoi(val);
	printf("============BMPCNT========%d\r\n", ret);
	return ret;
}


unsigned int get_avi_filecount()
{
	char val[32];
	unsigned int ret;
	memset(val, 0, 32);
	read_conf_value("AVICNT", val, "/dm368/filecnt.ini");
	ret = atoi(val);
	printf("============AVICNT========%d\r\n", ret);
	return ret;
}

