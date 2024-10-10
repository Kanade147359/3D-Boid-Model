# 3D-Boid-Model

1~3次元のBoid ModelのC++実装
低次元から、serialからmpiの順に実装していく

## インストールとビルド(予定)
このリポジトリをクローンし、以下の手順でビルドしてください。

```
git clone https://your-repository-url.git
cd boid_simulation
make
```

make コマンドを実行することで、プログラムがコンパイルされます。

## 実行方法
プログラムは、Boidの数を引数として実行できます。引数を指定しない場合、デフォルトのBoid数が使用されます。

### 例1: デフォルト設定で実行

```
./boid_simulation
```

### 例2: Boidの数を指定して実行
```
./boid_simulation 100
```

上記のコマンドでは、100個のBoidでシミュレーションが実行されます。

## ファイルの説明(予定)
boid.cpp : Boidの動作を定義する実装ファイル
boid.h : Boidのクラス定義ファイル
main.cpp : シミュレーションのエントリーポイント
