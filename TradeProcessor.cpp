#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <CSVToXMLConvert.h>

void ConvertTradeRecordFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecords objects[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }

        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }

        int tradeamount;
        if (!intGetFromString(fields[1], &tradeamount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double tradeprice;
        if (!toDouble(fields[2], &tradeprice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(objects[objectCount].SourceCurrency, fields[0], 3);
        strncpy(objects[objectCount].DestinationCurrency, fields[0] + 3, 3);
        objects[objectCount].Lots = tradeamount / LotSize;
        objects[objectCount].Price = tradeprice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("ConvertedTradeFile.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", objects[i].SC);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", objects[i].DC);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", objects[i].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", objects[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}
