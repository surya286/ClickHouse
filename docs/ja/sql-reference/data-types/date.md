---
toc_priority: 47
toc_title: "\u65E5\u4ED8"
---

# 日付 {#date}

日付型です。 1970-01-01 からの日数が2バイトの符号なし整数として格納されます。 UNIX時間の開始直後から、変換段階で定数として定義される上限しきい値までの値を格納できます（現在は2106年までですが、一年分を完全にサポートしているのは2105年までです）。

日付値は、タイムゾーンなしで格納されます。

[元の記事](https://clickhouse.com/docs/en/data_types/date/) <!--hide-->