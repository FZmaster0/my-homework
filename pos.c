#include <stdio.h>
#include <string.h>

#define MAX 100   /* 最多存 100 件商品 */

int main(void) {
    char   codes[MAX][16];   /* 条码 */
    char   names[MAX][32];   /* 名称 */
    double prices[MAX];      /* 价格 */
    int    count = 0;        /* 商品种数 */

    char line[256];          /* 存放用户输入的一行 */
    int  i;

    /*  第一步：读取商品文件 items.csv  */
    FILE *fp = fopen("items.csv", "r");
    if (fp == NULL) {
        /* 文件不存在，先创建它，写入题目给定的三件商品 */
        fp = fopen("items.csv", "w");
        if (fp == NULL) {
            printf("ERROR: cannot create items.csv\n");
            return 1;
        }
        fprintf(fp, "001,Cola,3.50\n");
        fprintf(fp, "002,Lollipop,0.50\n");
        fprintf(fp, "003,Noodles,6.00\n");
        fclose(fp);

        fp = fopen("items.csv", "r");   /* 再重新打开来读 */
    }

    /* 按逗号分隔读每一行：条码,名称,价格 */
    while (fscanf(fp, "%15[^,],%31[^,],%lf\n",
                  codes[count], names[count], &prices[count]) == 3) {
        count++;
    }
    fclose(fp);

    /* 打印欢迎信息 */
    printf("==============================================\n");
    printf("  711 Convenience Store - POS System\n");
    printf("  Commands: <barcode> / prices / quit\n");
    printf("==============================================\n");

    /*  第二步：主循环，反复读取用户输入 */
    while (1) {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;   /* 输入结束（Ctrl+D），退出 */
        }

        /* 去掉行尾的换行符 */
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') {
            continue;   /* 空行，重新输入 */
        }

        /* 退出命令 */
        if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0) {
            printf("Bye.\n");
            break;
        }

        /* 查看价格表 */
        if (strcmp(line, "prices") == 0) {
            printf("\n%-12s %-6s %8s\n", "Item", "No.", "Price");
            printf("------------------------\n");
            for (i = 0; i < count; i++) {
                printf("%-12s %-6s %8.2f\n", names[i], codes[i], prices[i]);
            }
            printf("\n");
            continue;
        }

        /* 其他输入一律当作条码处理。
         * 用 strtok 按空格把一行拆开，这样一行可以扫多个条码，
         * 比如 "001 003" 会分别显示可乐和面条。 */
        char *tok = strtok(line, " ");
        while (tok != NULL) {
            int found = 0;
            for (i = 0; i < count; i++) {
                if (strcmp(tok, codes[i]) == 0) {   /* 找到了这件商品 */
                    printf("%s %.2f\n", names[i], prices[i]);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("ERROR: code not found\n");
            }
            tok = strtok(NULL, " ");   /* 继续取下一个条码 */
        }
    }

    return 0;
}
