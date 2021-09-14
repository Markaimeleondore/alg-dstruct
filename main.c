#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
///////////////////////
#define mem (ll*)malloc(sizeof(ll))

/*
Условие:
Файл содержит следующую информацию (построчно): дата и средняя температура в этот день. Считать
файл в список таким образом, чтобы он оставался отсортированным по возрастанию средней
температуры (при совпадении средней температуры проводить сортировку следует по дате). Вывести на
экран информацию обо всех днях, в которых средняя температура оказалась ниже нуля. Ввести с
клавиатуры число и проверить, есть ли в списке день с такой средней температурой.
------------------
ТЗ:
    1.Сделать считывание из файла в формате структуры                                                           +
    2.Реализовать функцию ввода и печати                                                                        +
        2.1. Ввод должен ставить элементы по порядку неубывания, если температуры равны, сортировать по дате    +
        2.2. Печать только отрицательных элементов                                                              +
    3. Сделать проверку на наличие элемента в списке, ответить на запрос                                        +
    4. Сделать тесты функций
*/

typedef struct wowMark //Инициализируем структурку LinkedList(ll).
{
       int  temp;
       int  year;
       int  month;//number of month
       int  day;

    struct wowMark *next;
}   ll;

int comparator(int temp1, int temp2, int year1, int year2, int month1, int month2, int day1, int day2)
{
    if(temp1 < temp2)
    {
        return 1;
    }
    else if(temp1 == temp2 && year1 < year2)
    {
        return 1;
    }
    else if( temp1 == temp2 && year1 == year2 && month1 < month2)
    {
        return 1;
    }
    else if(temp1 == temp2 && year1 == year2 && month1 == month2 && day1 < day2)
    {
        return 1;
    }
    return 0;

}

void addElementInOrder(ll** head, int year_value, int month_value, int day_value, int temperature_value)
{
    ll* c = mem;
    c -> year = year_value;
    c -> month   = month_value;
    c -> day  = day_value;
    c -> temp = temperature_value;
    ll* cur;

    if((*head) == NULL)
    {
        c -> next = *head;
        *head = c;
    }
    else
    {
        cur = *head;

        while(cur -> next != NULL)
        {
            if(comparator(cur->next->temp, c->temp, cur->next->year, c->year, cur->next->month, c->month, cur->next->day, c->day))
                break;

            cur = cur -> next;
        }

        c -> next = cur -> next;

        cur ->  next = c;
    }
}



void printList(ll *head)
{

    if(head != NULL)
    {

        printList(head->next);
        if(head->temp < 0)
            {
                printf("Year:%d\nNumber of month:%d\nDay:%d\nTemperature:%d\n-----\n", head -> year, head -> month, head -> day, head -> temp);
            }
    }
}

int is_requested_temp_in_list(ll *head, int x)
{
    while(head != NULL)
    {
        if(head->temp == x)
        {
            printf("Hell, yea! This one is in the list!\n-----\nYear:%d\nNumber of month:%d\nDay:%d\nTemperature:%d\n-----\n", head -> year, head -> month, head -> day, head -> temp);
            return head->year;
        }
        else
            head = head -> next;
    }
    printf("There is nothing like that :(\n");
    return 0;
}
//Проверка на добавление
void ASSERT1_does___addElementInOrder___add_the_element(ll *head)
{
    addElementInOrder(&head, 1, 1, 1, 99);
    //ASSERT
    assert(head->temp == 99);
}
//Проверка на добавление в порядке убывания
void ASSERT2_does___addElementInOrder___add_the_element_in_non_descending_order_of_temperature_values(ll *head)
{
    addElementInOrder(&head, 1, 1, 1, 99);
    addElementInOrder(&head, 1, 1, 1, 98);
    //ASSERT
    assert(head->temp > head->next->temp);
}
//Проверки функции сравнения, вынес их в ASSERT3
// 1. По температуре
void ASSERT3_does_comparator_really_compare_temperature_case(void)
{
    assert(comparator(1, 2, 1, 1, 1, 1, 1, 1) == 1);
}
// 2. По году
void ASSERT3_does_comparator_really_compare_year_case(void)
{
    assert(comparator(1, 1, 1, 2, 1, 1, 1, 1) == 1);
}
// 3. По месяцу
void ASSERT3_does_comparator_really_compare_month_case(void)
{
    assert(comparator(1, 1, 1, 1, 1, 2, 1, 1) == 1);
}
// 4. По дню
void ASSERT3_does_comparator_really_compare_day_case(void)
{
    assert(comparator(1, 1, 1, 1, 1, 1, 1, 2) == 1);
}

//Проверка ответа на запрос
void ASSERT4_does___ans___seek_for_temperature_value_correctly_in_general(ll *head)
{
    addElementInOrder(&head, 1, 1, 1, 99);
    addElementInOrder(&head, 1, 1, 1, 98);
    addElementInOrder(&head, 1, 1, 1, 97);
    addElementInOrder(&head, 1, 1, 1, 96);
    addElementInOrder(&head, 1, 1, 1, 95);
    //ASSERTS
    assert(is_requested_temp_in_list(head, 99) == 1);
    assert(is_requested_temp_in_list(head, 1) == 0);
    //Erase results of assertion
    system("clear");
}

//Проверка порядка поиска ответа на запрос
void ASSERT5_the_order_of___ans___seeking_is_descent(ll *head)
{
    addElementInOrder(&head, 5, 1, 1, 99);
    addElementInOrder(&head, 4, 1, 1, 99);
    addElementInOrder(&head, 3, 1, 1, 99);
    //ASSERT
    assert(is_requested_temp_in_list(head, 99) == 5);
    //Erase results of assertion
    system("clear");
}




int main()
{
    //Logic block: FILE
    FILE *read_data;
    char name[] = "testCases.text";

    //Logic block: инициализируем Linked List
    ll *head = NULL;
    //TESTS
    ASSERT1_does___addElementInOrder___add_the_element(head);
    ASSERT2_does___addElementInOrder___add_the_element_in_non_descending_order_of_temperature_values(head);

    ASSERT3_does_comparator_really_compare_temperature_case();
    ASSERT3_does_comparator_really_compare_year_case();
    ASSERT3_does_comparator_really_compare_month_case();
    ASSERT3_does_comparator_really_compare_day_case();


    ASSERT4_does___ans___seek_for_temperature_value_correctly_in_general(head);
    ASSERT5_the_order_of___ans___seeking_is_descent(head);

    if((read_data = fopen(name, "r")))
        {
            int year, month, day, avg;
            addElementInOrder(&head, 0, 0, 0, 0);
            while(fscanf(read_data,"%d %d %d\n%d\n", &year, &month, &day, &avg) != EOF)
            {
                    addElementInOrder(&head, year, month, day, avg);


            }
            fclose(read_data);
            printf("-----\n");
            printList(head);

            int request;
            scanf("%d", &request);
            is_requested_temp_in_list(head ,request);




        }
    else
        {
            printf("Поменяйте название тестового файла :)");
            return 0;
        }
        getchar();
}
