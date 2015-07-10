# gen_feature & train

## gen_features

画像から色の特徴量を抽出する

### compile

```bash
$ cd cpp
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### usage
hogehogeフォルダの中に画像が連番で入っていることが想定されている。  
学習に使うデータには失敗が入っていても良い

```bash
$ ./cpp/build/gen_features --input=hogehoge --start=スタート番号 --end=最後の番号 > train.csv
```

(例)
```bash
$ ./cpp/build/gen_features --input=./images --start=800 --end=1295 > train.csv
```

`train.csv`が出来たら画像を確認しながら一番最後の列にラベルを振っていく。  
今回は以下のようにラベル付けした

0 -> eri  
1 -> hanayo  
2 -> honoka  
3 -> kotori  
4 -> maki  
5 -> niko  
6 -> nozomi  
7 -> rin  
8 -> umi  
9 -> etc  

## train

抽出した特徴量から上手く分類できるような学習器を作成する。  


### compile
`gen_features`で入力するファイル数を変えたら中身のdefineを変えること。  

`gen_feature`で一緒に出来上がる

### usage
面倒なので学習データとテストデータに同じデータを用いる。
`train.csv`には`gen_features`で作ったcsvファイルにラベルを加えたもの．

```bash
$ ./cpp/build/train train-labeled.csv train-labeled.csv
```

`params.xml`が出来上がる．
