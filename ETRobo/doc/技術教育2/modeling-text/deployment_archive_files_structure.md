# 技術教育資料の配布アーカイブの構成について

技術教育で参加者に配布するテキスト、サンプルコード、サンプルモデルは、この文書にあるような構成にしてあります。

このうち、「開発プロセスに沿って開発する」に使う資料は、次の使い方に対応するために、配布物の一部はパスワードつきのzipファイルになっています。
* 段階的に解答例を開示しながら進める
* 初めから解答例を閲覧しながら進める
* 参加者自身が自習する

## 配布物の構成

### 受講生に配布するもの

1. 開発環境・要素技術セミナー用（技術教育1の午前用）
 * `devEnv_and_underlyingTech.pdf`
1. 「要素技術とモデルを開発に使おう」に使うもの（技術教育1の午後用）
 * `beginners.zip`
1. 「開発プロセスに沿って開発する」に使うもの（技術教育2用）
 * `chap-00-04.zip`、`chap-05-07.zip`、`chap-08-apx.zip`
 * `chap-05-ans.zip`、`chap-06-ans.zip`、`chap-07-ans.zip` （これらはパスワードつき）

「開発プロセスに沿って開発する」のテキストは、演習の結果の一部をノート型図形でマスクしてあります（`chap-00-04.zip`、`chap-05-07.zip`、`chap-08-apx.zip`）。

「開発プロセスに沿って開発する」の演習結果が見えるようにマスクを外してあるテキスト、解答例のモデルとコードのサンプルは、章ごとにパスワードつきのzipファイルにまとめてあります（`chap-05-ans.zip`、`chap-06-ans.zip`、`chap-07-ans.zip`）。閲覧するには、次に示すパスワードファイルに記載されたパスワードを使って解凍する必要があります。

### 実行委員に配布するもの

1. 「開発プロセスに沿って開発する」の解答例解凍用パスワードファイル
 * `chap-05-07-ans.passwords.txt`

このファイルには、 `chap-05-ans.zip`、`chap-06-ans.zip`、`chap-07-ans.zip` の各ファイルの解凍用パスワード3つが記載されています。

実行委員は、その地区の技術教育の進め方に応じて、このパスワードファイルを自分たちが使うか、事前に配布するか、といった判断をして技術教育を運用します。
 

## 開発環境・要素技術セミナー用（技術教育1の午前）

`devEnv_and_underlyingTech.pdf`  を配布します。

## 「要素技術とモデルを開発に使おう」用（技術教育1の午後）

「要素技術とモデルを開発に使おう」は、2016年から提供する初学者向けの技術教育です。

beginnersディレクトリ全体をそのままzipします（パスワードなし）。

```shell
beginners.zip
├── README.md
├── asta/*
├── asta-template/*
├── sample_code/*
└── text/*
```

## 「開発プロセスに沿って開発する」用（技術教育2）


「開発プロセスに沿って開発する」は、2015年度までの旧「モデリング入門」のことです。

GitHubの構造のままではなく、配布用に再編成してzipしてあります。

提供するzipファイルには、パスワードありのものとなしのものがあります。

### chap-00-04.zip

chap-00からchap-04までをサイズ最適化した上でpdf化したものと、sample_code/etrobo_tr と asta/etrobo_tr.astaでzipしてあります。

```shell
chap-00-04.zip
├── pdf
│   ├── chap-00_c.pdf
│   ├── chap-01_c.pdf
│   ├── chap-02_c.pdf
│   ├── chap-03_c.pdf
│   └── chap-04_c.pdf
├── asta
│   └── etrobo_tr.asta
└── sample_code
    ├── Makefile
    └── etrobo_tr/*
```

### chap-05-07.zip

chap-05からchap-06までのpdf化したものでzipしてあります（このzipはテキストだけ）。

```shell
chap-05-07.zip
└── pdf
    ├── chap-05_c.pdf
    ├── chap-06_c.pdf
    └── chap-07_c.pdf
```

### chap-05-ans.zip（パスワードあり）

chap-05-openedをpdf化したものと、sample_code/etrobo_tr_ex1 と asta/etrobo_tr_ex1.asta でzipし、zipファイルにはパスワードをかけてあります。

演習で作成するモデル図ファイル名はetrobo_tr_1.asta、コードのディレクトリは、etrobo_tr_1とします。解答例として配るのは、etrobo_tr_ex1.asta、etrobo_tr_ex1とし、演習結果の名前と解答例の名前が被らないようにしてあります。

```shell
chap-05-ans.zip (w/password)
├── pdf
│   └── chap-05-opened_c.pdf
├── asta
│   └── etrobo_tr_ex1.asta
└── sample_code
    └── etrobo_tr_ex1/*
```

### chap-06-ans.zip（パスワードあり）

chap-06-openedをpdf化したものと、sample_code/etrobo_tr_ex2 と asta/etrobo_tr_ex2.asta でzipし、zipファイルにはパスワードをかけてあります。

演習で作成するモデル図ファイル名はetrobo_tr_2.asta、コードのディレクトリは、etrobo_tr_2とします。解答例として配るetrobo_tr_ex2.asta、etrobo_tr_ex2とし、演習結果の名前と解答例の名前が被らないようにしてあります。

```shell
chap-06-ans.zip (w/password)
├── pdf
│   └── chap-06-opened_c.pdf
├── asta
│   └── etrobo_tr_ex2.asta
└── sample_code
    └── etrobo_tr_ex2/*
```

### chap-07-ans.zip（パスワードあり）

chap-07-openedをpdf化したものと、sample_code/etrobo_tr_ex7 と asta/etrobo_tr_ex7.asta でzipし、zipファイルにはパスワードをかけてあります。

演習で作成するモデル図ファイル名はetrobo_tr_3.asta、コードのディレクトリは、etrobo_tr_3とします。解答例として配るetrobo_tr_ex3.asta、etrobo_tr_ex3とし、演習結果の名前と解答例の名前が被らないようにしてあります。

```shell
chap-07-ans.zip (w/password)
├── pdf
│   └── chap-07-opened_c.pdf
├── asta
│   └── etrobo_tr_ex3.asta
└── sample_code
    └── etrobo_tr_ex3/*
```

### chap-08-apx.zip

chap-08からchap-appendixまでをpdf化したものでzipすます（このzipはテキストだけ）。

```shell
chap-08-apx.zip
└── pdf
    ├── chap-08_c.pdf
    ├── chap-09_c.pdf
    └── chap-appendix_c.pdf
```

## パスワードファイル（実行委員用）

実行委員には、次の3つのzipファイルのそれぞれにかけたパスワードの入った、通常のテキストファイルを提供します。

chap-05-07-ans.passwords.txt

* chap-05-ans.zip解凍用パスワード
* chap-06-ans.zip解凍用パスワード
* chap-07-ans.zip解凍用パスワード
