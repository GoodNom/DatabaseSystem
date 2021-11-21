#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "127.0.0.1";
const char* user = "root";
const char* pw = "20171630";
const char* db = "proj2";

int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	char* buffer = NULL;
	long long int size;
	int type, subtype, flag = 0, select = 0, k;
	char temp[50], date1[10], date2[10];

	/* create_insert.txt file open and read */
	FILE* fp = fopen("20171630_1.txt", "r");

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*)malloc(size + 1);
	memset(buffer, 0, size + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, size, 1, fp);

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}
		int state = 0;

		/* create and insert */
		const char* init = strtok(buffer, ";");
		while (init != NULL) {
			state = 0;
			state = mysql_query(connection, init);
			init = strtok(NULL, ";");
		}

		/* select query */
		char query[2024];
		while (1) {
			flag = 0;
			printf("\n\n------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t6. TYPE 6\n");
			printf("\t7. TYPE 7\n");
			printf("\t0. QUIT\n");
			printf("----------------------------------\n");
			printf("Select Number : ");
			scanf(" %d", &type);
			getchar();
			switch (type) {
			case 1:
				printf("\n\n");
				printf("---- TYPE 1 ----\n\n");
				printf("** Show the sales trends for a particular brand over the past k years. **\n");
				printf(" Which K? : ");
				scanf(" %d", &k);
				getchar();
				printf(" Which brand? : ");
				fgets(temp, sizeof(temp), stdin);
				temp[strlen(temp) - 1] = '\0';
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as  (select brand_name,year(sell_date) as year,customer_id from vehicle natural join model where year(sell_date) >= year(NOW() - interval %d year) and brand_name = '%s') select year, count(year) as sales from trends group by year order by year desc", k, temp);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nyear  sales\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-4s  %-5s\n", sql_row[0], sql_row[1]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");

				printf("---- Subtypes in TYPE 1 ----\n");
				printf("\t1. TYPE 1-1\n");
				printf("\t0. QUIT\n");
				printf("----------------------------\n");
				printf("Select Number : ");
				scanf(" %d", &subtype);
				if (subtype != 1) break;

				printf("\n---- TYPE 1-1 ----\n\n");
				printf("** Then break these data out by gender of the buyer. **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as (select brand_name,year(sell_date) as year,customer_id from vehicle natural join model where year(sell_date) >= year(NOW() - interval %d year) and brand_name = '%s') select year, count(year) as sales, count(case when gender = 'F' then 1 end) as Female,count(case when gender = 'M' then 1 end) as Male from trends natural join customer group by year order by year desc", k, temp);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nyear  sales  Female  Male\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-4s  %-5s  %-6s  %-4s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");

				printf("---- Subtypes in TYPE 1-1 ----\n");
				printf("\t1. TYPE 1-1-1\n");
				printf("\t0. QUIT\n");
				printf("----------------------------\n");
				printf("Select Number : ");
				scanf(" %d", &subtype);
				if (subtype != 1) break;

				printf("\n---- TYPE 1-1-1 ----\n\n");
				printf("**  Then by income range. **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as  (select brand_name, year(sell_date) as year, customer_id from vehicle natural join model where year(sell_date) >= year(NOW() - interval %d year) and brand_name = '%s') select year, count(year) as sales, count(case when gender = 'F' then 1 end) as Female, count(case when gender = 'M' then 1 end) as Male, count(case when annual_income >= 0 and annual_income < 100000000 and gender = 'F' then 1 end) as '(F) 0~100000000', count(case when annual_income >= 0 and annual_income < 100000000 and gender = 'M' then 1 end) as '(M) 0~100000000', count(case when annual_income >= 100000000 and annual_income < 200000000 and gender = 'F' then 1 end) as '(F) 0~200000000', count(case when annual_income >= 100000000 and annual_income < 200000000 and gender = 'M' then 1 end) as '(M) 0~200000000', count(case when annual_income >= 200000000 and gender = 'F' then 1 end) as '(F) 200000000~', count(case when annual_income >= 200000000 and gender = 'M' then 1 end) as '(M) 200000000~' from trends natural join customer group by year order by year desc", k, temp);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nyear  sales  Female  Male  <F>0~1billion(\\)  <M>0~1billion(\\)  <F>1~2billion(\\)  <M>1~2billion(\\)  <F>2billion(\\)~  <M>2billion(\\)~\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-4s  %-5s  %-6s  %-4s  %-16s  %-16s  %-16s  %-16s  %-15s  %-15s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9]);
					}
					mysql_free_result(sql_result);
				}
				memset(query, 0, sizeof(query));
				break;
			case 2:
				printf("\n\n");
				printf("---- TYPE 2 ----\n\n");
				printf("** Show sales trends for various brands over the past k months. **\n");
				printf(" Which K? : ");
				scanf(" %d", &k);
				getchar();
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as (select brand_name, DATE_FORMAT(sell_date, '%%Y-%%m') as month from vehicle natural join model where date_format(sell_date,'%%Y-%%m') >= date_format((NOW() - interval %d month),'%%Y-%%m') order by brand_name, sell_date) select*, count(month) as sales from trends group by brand_name, month order by brand_name, month", k);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nbrand_name   month    sales\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-11s  %-7s  %-5s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");

				printf("---- Subtypes in TYPE 2 ----\n");
				printf("\t1. TYPE 2-1\n");
				printf("\t0. QUIT\n");
				printf("----------------------------\n");
				printf("Select Number : ");
				scanf(" %d", &subtype);
				if (subtype != 1) break;

				printf("\n---- TYPE 2-1 ----\n\n");
				printf("** Then break these data out by gender of the buyer.  **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as (select brand_name, DATE_FORMAT(sell_date, '%%Y-%%m') as month, customer_id from vehicle natural join model where date_format(sell_date,'%%Y-%%m') >= date_format((NOW() - interval %d month),'%%Y-%%m') order by brand_name, sell_date) select brand_name, month, count(month) as sales, count(case when gender = 'F' then 1 end) as Female, count(case when gender = 'M' then 1 end) as Male from trends natural join customer group by brand_name, month order by brand_name, month", k);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nbrand_name   month    sales  Female  Male\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-11s  %-7s  %-5s  %-6s  %-4s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");

				printf("---- Subtypes in TYPE 2-1 ----\n");
				printf("\t1. TYPE 2-1-1\n");
				printf("\t0. QUIT\n");
				printf("----------------------------\n");
				printf("Select Number : ");
				scanf(" %d", &subtype);
				if (subtype != 1) break;

				printf("\n---- TYPE 2-1-1 ----\n\n");
				printf("** Then by income range.  **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "with trends as (select brand_name, DATE_FORMAT(sell_date, '%%Y-%%m') as month, customer_id from vehicle natural join model where date_format(sell_date,'%%Y-%%m') >= date_format((NOW() - interval %d month),'%%Y-%%m') order by brand_name, sell_date) select brand_name, month, count(month) as sales, 	count(case when gender = 'F' then 1 end) as Female, count(case when gender = 'M' then 1 end) as Male, count(case when annual_income >= 0 and annual_income < 100000000 and gender = 'F' then 1 end) as '(F) 0~100000000', count(case when annual_income >= 0 and annual_income < 100000000 and gender = 'M' then 1 end) as '(M) 0~100000000', count(case when annual_income >= 100000000 and annual_income < 200000000 and gender = 'F' then 1 end) as '(F) 0~200000000', count(case when annual_income >= 100000000 and annual_income < 200000000 and gender = 'M' then 1 end) as '(M) 0~200000000', count(case when annual_income >= 200000000 and gender = 'F' then 1 end) as '(F) 200000000~', count(case when annual_income >= 200000000 and gender = 'M' then 1 end) as '(M) 200000000~' from trends natural join customer group by brand_name, month order by brand_name, month", k);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\nbrand_name   month    sales  Female  Male  <F>0~1billion(\\)  <M>0~1billion(\\)  <F>1~2billion(\\)  <M>1~2billion(\\)  <F>2billion(\\)~  <M>2billion(\\)~\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-11s  %-7s  %-5s  %-6s  %-4s  %-16s  %-16s  %-16s  %-16s  %-15s  %-15s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6], sql_row[7], sql_row[8], sql_row[9], sql_row[10]);
					}
					mysql_free_result(sql_result);
				}
				break;
			case 3:
				printf("\n\n");
				printf("---- TYPE 3 ----\n\n");
				printf("** Find that transmissions made by supplier (company name) between two given dates are defective. **\n");
				printf(" Which date1? : ");
				fgets(date1, sizeof(date1), stdin);
				date1[strlen(date1) - 1] = '\0';
				printf(" Which date2? : ");
				fgets(date2, sizeof(date2), stdin);
				date2[strlen(date2) - 1] = '\0';
				printf(" Which supplier? : ");
				fgets(temp, sizeof(temp), stdin);
				temp[strlen(temp) - 1] = '\0';
				memset(query, 0, sizeof(query));
				sprintf(query, "select component,supply_date, supplier_name from parts natural join supply_plant where component = 'transmission' and supply_date > %s and supply_date < %s and supplier_name = '%s'", date1, date2, temp);

				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\ncomponent      supply_date  supplier_name\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-13s  %-11s  %-13s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}
				printf("\n\n");

				printf("---- Subtypes in TYPE 3 ----\n");
				printf("\t1. TYPE 3-1\n");
				printf("\t2. TYPE 3-2\n");
				printf("\t0. QUIT\n");
				printf("----------------------------\n");
				printf("Select Number : ");
				scanf(" %d", &subtype);
				if (subtype == 1) {
					printf("---- TYPE 3-1 ----\n\n");
					printf("**  Find the VIN of each car containing such a transmission and the customer to which it was sold. **\n");
					memset(query, 0, sizeof(query));
					sprintf(query, "with defective as (select S_ID, component, supply_date, supplier_name from parts natural join supply_plant where component = 'transmission' and supply_date > %s and supply_date < %s and supplier_name = '%s') select VIN, customer_id, customer_name from(defective natural join consist) natural join vehicle natural join customer", date1, date2, temp);

					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						printf("\nVIN                customer_id  customer_name\n");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%-17s  %-11s  %-13s\n", sql_row[0], sql_row[1], sql_row[2]);
						}
						mysql_free_result(sql_result);
					}
				}
				else if (subtype == 2) {
					printf("---- TYPE 3-2 ----\n\n");
					printf("**   Find the dealer who sold the VIN and transmission for each vehicle  containing these transmissions. **\n");
					memset(query, 0, sizeof(query));
					sprintf(query, "with defective as 	(select S_ID, component, supply_date, supplier_name from parts natural join supply_plant where component = 'transmission' and supply_date > %s and supply_date < %s and supplier_name = '%s') select VIN, dealer_id, dealer_name from(defective natural join consist) natural join vehicle natural join dealer", date1, date2, temp);

					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						printf("\nVIN                dealer_id  dealer_name\n");
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf("%-17s  %-9s  %-11s\n", sql_row[0], sql_row[1], sql_row[2]);
						}
						mysql_free_result(sql_result);
					}
				}
				break;
			case 4:
				printf("\n\n");
				printf("---- TYPE 4 ----\n\n");
				printf("** Find the top k brands by dollar-amount sold by the year. **\n");
				printf(" Which K? : ");
				scanf(" %d", &k);
				getchar();
				memset(query, 0, sizeof(query));
				sprintf(query, "create view dollar_amount as (select year(sell_date) as year, sum(price)as price, brand_name, customer_id from vehicle natural join model natural join total_price group by year(sell_date), brand_name having customer_id is not null);");
				state = 0;
				state = mysql_query(connection, query);

				memset(query, 0, sizeof(query));
				sprintf(query, "with dollar_amount_rank as (select year, price, brand_name, (@VRank := @Rank + 1), (@real_rank := IF(@last > price, @real_rank:= @real_rank + 1, @real_rank)), (@real_rank := IF(@prev_year < year, @real_rank:= 1, @real_rank)) as real_rank, (@last := price), (@prev_year := year) from dollar_amount, (select @vRank := 0, @real_rank := 1, @last := 0) as b order by year, price desc) select year, brand_name, real_rank as 'rank' from dollar_amount_rank where real_rank <= %d order by year desc;", k);
				state = 0;
				state = mysql_query(connection, query);

				if (state == 0)
				{
					printf("\nyear  brand_name   rank\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-4s  %-11s  %-4s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}

				memset(query, 0, sizeof(query));
				sprintf(query, "drop view if exists dollar_amount;");
				state = 0;
				state = mysql_query(connection, query);

				break;
			case 5:
				printf("\n\n");
				printf("---- TYPE 5 ----\n\n");
				printf("** Find the top k brands by unit sales by the year. **\n");
				printf(" Which K? : ");
				scanf(" %d", &k);
				getchar();
				memset(query, 0, sizeof(query));
				sprintf(query, "create view unit_sale as (select year(sell_date) as year, count(brand_name)as cnt,brand_name, customer_id from vehicle natural join model group by year(sell_date),brand_name having customer_id is not null);");
				state = 0;
				state = mysql_query(connection, query);

				memset(query, 0, sizeof(query));
				sprintf(query, "with unit_sale_rank as (select year,cnt,brand_name,  ( @VRank := @Rank + 1 ), ( @real_rank := IF ( @last > cnt, @real_rank:=@real_rank+1, @real_rank)) , ( @real_rank := IF(@prev_year < year,@real_rank:=1,@real_rank)) as real_rank, ( @last := cnt ), ( @prev_year := year ) from unit_sale, (select @vRank := 0,@real_rank :=1,@last:=0) as b order by year, cnt desc) select year, brand_name, real_rank as 'rank' from unit_sale_rank where real_rank <= %d order by year desc;", k);
				state = 0;
				state = mysql_query(connection, query);

				if (state == 0)
				{
					printf("\nyear  brand_name   rank\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-4s  %-11s  %-4s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}

				memset(query, 0, sizeof(query));
				sprintf(query, "drop view if exists unit_sale;");
				state = 0;
				state = mysql_query(connection, query);
				break;
			case 6:
				printf("\n\n");
				printf("---- TYPE 6 ----\n\n");
				printf("** In what month(s) do convertibles sell best? **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "create view sale_convertible as (select sell_date from vehicle natural join model where body_style = 'convertible' and sell_date is not null);");
				state = 0;
				state = mysql_query(connection, query);

				memset(query, 0, sizeof(query));
				sprintf(query, "with month_sales as (select month(sell_date)as month, count(month(sell_date)) as sales from sale_convertible group by month(sell_date) order by month(sell_date)) select month from month_sales where sales = (select max(sales) from month_sales);");
				state = 0;
				state = mysql_query(connection, query);

				if (state == 0)
				{
					printf("\nmonth\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-2s\n", sql_row[0]);
					}
					mysql_free_result(sql_result);
				}

				memset(query, 0, sizeof(query));
				sprintf(query, "drop view if exists sale_convertible;");
				state = 0;
				state = mysql_query(connection, query);
				break;
			case 7:
				printf("\n\n");
				printf("---- TYPE 7 ----\n\n");
				printf("** Find those dealers who keep a vehicle in inventory for the longest average time. **\n");
				memset(query, 0, sizeof(query));
				sprintf(query, "with inventory as (select dealer_id,dealer_name, (case when sell_date is not null then DATEDIFF(sell_date,get_date) else DATEDIFF(NOW(),get_date)end) as date from vehicle natural join dealer) select dealer_name from inventory where date = (select max(date) from inventory)");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("\ndealer_name\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%-9s\n", sql_row[0]);
					}
					mysql_free_result(sql_result);
				}
				break;
			case 0:
				flag = 1;
				break;
			default:
				flag = 0;
				break;
			}

			if (flag) break;
		}


		/*  delete and drop  */
		FILE* fp2 = fopen("20171630_2.txt", "r");
		char* buffer2 = NULL;

		fseek(fp2, 0, SEEK_END);
		size = ftell(fp2);
		buffer2 = (char*)malloc(size + 1);
		memset(buffer2, 0, size + 1);
		fseek(fp2, 0, SEEK_SET);
		fread(buffer2, size, 1, fp2);


		const char* end = strtok(buffer2, ";");
		while (end != NULL) {
			state = 0;
			state = mysql_query(connection, end);
			end = strtok(NULL, ";");
		}


		mysql_close(connection);
		fclose(fp2);
		free(buffer2);
	}
	fclose(fp);
	free(buffer);

	return 0;
}