//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　	         汎用関数, データ型                              **//
//**                                                                                     **//
//*****************************************************************************************//

#include "Function.h"

void MatrixIdentity(MATRIX *mat){
	mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void MatrixScaling(MATRIX *mat, float sizex, float sizey, float sizez){
	mat->_11 = sizex; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = sizey; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sizez; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void MatrixRotationX(MATRIX *mat, float theta){
	float the = theta * 3.14f / 180.0f;
	mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = (float)cos(the); mat->_23 = (float)sin(the); mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = -(float)sin(the); mat->_33 = (float)cos(the); mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void MatrixRotationY(MATRIX *mat, float theta){
	float the = theta * 3.14f / 180.0f;
	mat->_11 = (float)cos(the); mat->_12 = 0.0f; mat->_13 = -(float)sin(the); mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = (float)sin(the); mat->_32 = 0.0f; mat->_33 = (float)cos(the); mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void MatrixRotationZ(MATRIX *mat, float theta){
	float the = theta * 3.14f / 180.0f;
	mat->_11 = (float)cos(the); mat->_12 = (float)sin(the); mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = -(float)sin(the); mat->_22 = (float)cos(the); mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void MatrixTranslation(MATRIX *mat, float movx, float movy, float movz){
	mat->_11 = 1.0f; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = 1.0f; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = movx; mat->_42 = movy; mat->_43 = movz; mat->_44 = 1.0f;
}

void MatrixAddition(MATRIX *mat, MATRIX *mat1, MATRIX *mat2){
	mat->_11 = mat1->_11 + mat2->_11;
	mat->_12 = mat1->_12 + mat2->_12;
	mat->_13 = mat1->_13 + mat2->_13;
	mat->_14 = mat1->_14 + mat2->_14;

	mat->_21 = mat1->_21 + mat2->_21;
	mat->_22 = mat1->_22 + mat2->_22;
	mat->_23 = mat1->_23 + mat2->_23;
	mat->_24 = mat1->_24 + mat2->_24;

	mat->_31 = mat1->_31 + mat2->_31;
	mat->_32 = mat1->_32 + mat2->_32;
	mat->_33 = mat1->_33 + mat2->_33;
	mat->_34 = mat1->_34 + mat2->_34;

	mat->_41 = mat1->_41 + mat2->_41;
	mat->_42 = mat1->_42 + mat2->_42;
	mat->_43 = mat1->_43 + mat2->_43;
	mat->_44 = mat1->_44 + mat2->_44;
}

void MatrixMultiply(MATRIX *mat, MATRIX *mat1, MATRIX *mat2){
	mat->_11 = mat1->_11 * mat2->_11 + mat1->_12 * mat2->_21 + mat1->_13 * mat2->_31 + mat1->_14 * mat2->_41;
	mat->_12 = mat1->_11 * mat2->_12 + mat1->_12 * mat2->_22 + mat1->_13 * mat2->_32 + mat1->_14 * mat2->_42;
	mat->_13 = mat1->_11 * mat2->_13 + mat1->_12 * mat2->_23 + mat1->_13 * mat2->_33 + mat1->_14 * mat2->_43;
	mat->_14 = mat1->_11 * mat2->_14 + mat1->_12 * mat2->_24 + mat1->_13 * mat2->_34 + mat1->_14 * mat2->_44;

	mat->_21 = mat1->_21 * mat2->_11 + mat1->_22 * mat2->_21 + mat1->_23 * mat2->_31 + mat1->_24 * mat2->_41;
	mat->_22 = mat1->_21 * mat2->_12 + mat1->_22 * mat2->_22 + mat1->_23 * mat2->_32 + mat1->_24 * mat2->_42;
	mat->_23 = mat1->_21 * mat2->_13 + mat1->_22 * mat2->_23 + mat1->_23 * mat2->_33 + mat1->_24 * mat2->_43;
	mat->_24 = mat1->_21 * mat2->_14 + mat1->_22 * mat2->_24 + mat1->_23 * mat2->_34 + mat1->_24 * mat2->_44;

	mat->_31 = mat1->_31 * mat2->_11 + mat1->_32 * mat2->_21 + mat1->_33 * mat2->_31 + mat1->_34 * mat2->_41;
	mat->_32 = mat1->_31 * mat2->_12 + mat1->_32 * mat2->_22 + mat1->_33 * mat2->_32 + mat1->_34 * mat2->_42;
	mat->_33 = mat1->_31 * mat2->_13 + mat1->_32 * mat2->_23 + mat1->_33 * mat2->_33 + mat1->_34 * mat2->_43;
	mat->_34 = mat1->_31 * mat2->_14 + mat1->_32 * mat2->_24 + mat1->_33 * mat2->_34 + mat1->_34 * mat2->_44;

	mat->_41 = mat1->_41 * mat2->_11 + mat1->_42 * mat2->_21 + mat1->_43 * mat2->_31 + mat1->_44 * mat2->_41;
	mat->_42 = mat1->_41 * mat2->_12 + mat1->_42 * mat2->_22 + mat1->_43 * mat2->_32 + mat1->_44 * mat2->_42;
	mat->_43 = mat1->_41 * mat2->_13 + mat1->_42 * mat2->_23 + mat1->_43 * mat2->_33 + mat1->_44 * mat2->_43;
	mat->_44 = mat1->_41 * mat2->_14 + mat1->_42 * mat2->_24 + mat1->_43 * mat2->_34 + mat1->_44 * mat2->_44;
}

void swap(float *a, float *b){
	float tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void MatrixTranspose(MATRIX *mat){
	swap(&mat->_12, &mat->_21);
	swap(&mat->_13, &mat->_31);
	swap(&mat->_14, &mat->_41);
	swap(&mat->_23, &mat->_32);
	swap(&mat->_24, &mat->_42);
	swap(&mat->_34, &mat->_43);
}

void MatrixLookAtLH(MATRIX *mat, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	//z軸
	float zx = x2 - x1;
	float zy = y2 - y1;
	float zz = z2 - z1;
	//正規化
	float zzz = (float)sqrt(zx * zx + zy * zy + zz * zz);
	if (zzz != 0.0f){
		zx = zx / zzz;
		zy = zy / zzz;
		zz = zz / zzz;
	}

	//x軸(外積)
	float xx = y3 * zz - z3 * zy;
	float xy = z3 * zx - x3 * zz;
	float xz = x3 * zy - y3 * zx;
	float xxx = (float)sqrt(xx * xx + xy * xy + xz * xz);
	if (xxx != 0.0f){
		xx = xx / xxx;
		xy = xy / xxx;
		xz = xz / xxx;
	}

	//y軸(外積)
	float yx = zy * xz - zz * xy;
	float yy = zz * xx - zx * xz;
	float yz = zx * xy - zy * xx;

	//平行移動(内積)
	float mx = -(x1 * xx + y1 * xy + z1 * xz);
	float my = -(x1 * yx + y1 * yy + z1 * yz);
	float mz = -(x1 * zx + y1 * zy + z1 * zz);

	mat->_11 = xx; mat->_12 = yx; mat->_13 = zx; mat->_14 = 0.0f;
	mat->_21 = xy; mat->_22 = yy; mat->_23 = zy; mat->_24 = 0.0f;
	mat->_31 = xz; mat->_32 = yz; mat->_33 = zz; mat->_34 = 0.0f;
	mat->_41 = mx; mat->_42 = my; mat->_43 = mz; mat->_44 = 1.0f;
}

void MatrixPerspectiveFovLH(MATRIX *mat, float theta, float aspect, float Near, float Far){
	float the = theta * 3.14f / 180.0f;
	//透視変換後y方向スケーリング
	float sy = 1.0f / ((float)tan(the / 2.0f));
	//x方向スケーリング
	float sx = sy / aspect;
	//z方向スケーリング
	float sz = Far / (Far - Near);

	mat->_11 = sx; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = sy; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = sz; mat->_34 = 1.0f;
	mat->_41 = 0.0f; mat->_42 = 0.0f; mat->_43 = -(sz * Near); mat->_44 = 0.0f;
}

void VectorMatrixMultiply(VECTOR3 *v, MATRIX *mat){
	float x = v->x;
	float y = v->y;
	float z = v->z;
	float w;

	v->x = x * mat->_11 + y * mat->_21 + z * mat->_31 + mat->_41;
	v->y = x * mat->_12 + y * mat->_22 + z * mat->_32 + mat->_42;
	v->z = x * mat->_13 + y * mat->_23 + z * mat->_33 + mat->_43;
	w = x * mat->_14 + y * mat->_24 + z * mat->_34 + mat->_44;
	v->x /= w;
	v->y /= w;
	v->z /= w;
}

//MatrixInverse内で使用
double CalDetMat4x4(MATRIX *m) {

	return m->_11 * m->_22 * m->_33 * m->_44 + m->_11 * m->_23 * m->_34 * m->_42 + m->_11 * m->_24 * m->_32 * m->_43
		+ m->_12 * m->_21 * m->_34 * m->_43 + m->_12 * m->_23 * m->_31 * m->_44 + m->_12 * m->_24 * m->_33 * m->_41
		+ m->_13 * m->_21 * m->_32 * m->_44 + m->_13 * m->_22 * m->_34 * m->_41 + m->_13 * m->_24 * m->_31 * m->_42
		+ m->_14 * m->_21 * m->_33 * m->_42 + m->_14 * m->_22 * m->_31 * m->_43 + m->_14 * m->_23 * m->_32 * m->_41
		- m->_11 * m->_22 * m->_34 * m->_43 - m->_11 * m->_23 * m->_32 * m->_44 - m->_11 * m->_24 * m->_33 * m->_42
		- m->_12 * m->_21 * m->_33 * m->_44 - m->_12 * m->_23 * m->_34 * m->_41 - m->_12 * m->_24 * m->_31 * m->_43
		- m->_13 * m->_21 * m->_34 * m->_42 - m->_13 * m->_22 * m->_31 * m->_44 - m->_13 * m->_24 * m->_32 * m->_41
		- m->_14 * m->_21 * m->_32 * m->_43 - m->_14 * m->_22 * m->_33 * m->_41 - m->_14 * m->_23 * m->_31 * m->_42;
}

bool MatrixInverse(MATRIX *invm, MATRIX *m) {

	double det = CalDetMat4x4(m);
	if (fabs(det) < 0) {
		return false;
	}
	else {
		float inv_det = (float)(1.0 / det);

		invm->_11 = inv_det*(m->_22 * m->_33 * m->_44 + m->_23 * m->_34 * m->_42 + m->_24 * m->_32 * m->_43 - m->_22 * m->_34 * m->_43 - m->_23 * m->_32 * m->_44 - m->_24 * m->_33 * m->_42);
		invm->_12 = inv_det*(m->_12 * m->_34 * m->_43 + m->_13 * m->_32 * m->_44 + m->_14 * m->_33 * m->_42 - m->_12 * m->_33 * m->_44 - m->_13 * m->_34 * m->_42 - m->_14 * m->_32 * m->_43);
		invm->_13 = inv_det*(m->_12 * m->_23 * m->_44 + m->_13 * m->_24 * m->_42 + m->_14 * m->_22 * m->_43 - m->_12 * m->_24 * m->_43 - m->_13 * m->_22 * m->_44 - m->_14 * m->_23 * m->_42);
		invm->_14 = inv_det*(m->_12 * m->_24 * m->_33 + m->_13 * m->_22 * m->_34 + m->_14 * m->_23 * m->_32 - m->_12 * m->_23 * m->_34 - m->_13 * m->_24 * m->_32 - m->_14 * m->_22 * m->_33);

		invm->_21 = inv_det*(m->_21 * m->_34 * m->_43 + m->_23 * m->_31 * m->_44 + m->_24 * m->_33 * m->_41 - m->_21 * m->_33 * m->_44 - m->_23 * m->_34 * m->_41 - m->_24 * m->_31 * m->_43);
		invm->_22 = inv_det*(m->_11 * m->_33 * m->_44 + m->_13 * m->_34 * m->_41 + m->_14 * m->_31 * m->_43 - m->_11 * m->_34 * m->_43 - m->_13 * m->_31 * m->_44 - m->_14 * m->_33 * m->_41);
		invm->_23 = inv_det*(m->_11 * m->_24 * m->_43 + m->_13 * m->_21 * m->_44 + m->_14 * m->_23 * m->_41 - m->_11 * m->_23 * m->_44 - m->_13 * m->_24 * m->_41 - m->_14 * m->_21 * m->_43);
		invm->_24 = inv_det*(m->_11 * m->_23 * m->_34 + m->_13 * m->_24 * m->_31 + m->_14 * m->_21 * m->_33 - m->_11 * m->_24 * m->_33 - m->_13 * m->_21 * m->_34 - m->_14 * m->_23 * m->_31);

		invm->_31 = inv_det*(m->_21 * m->_32 * m->_44 + m->_22 * m->_34 * m->_41 + m->_24 * m->_31 * m->_42 - m->_21 * m->_34 * m->_42 - m->_22 * m->_31 * m->_44 - m->_24 * m->_32 * m->_41);
		invm->_32 = inv_det*(m->_11 * m->_34 * m->_42 + m->_12 * m->_31 * m->_44 + m->_14 * m->_32 * m->_41 - m->_11 * m->_32 * m->_44 - m->_12 * m->_34 * m->_41 - m->_14 * m->_31 * m->_42);
		invm->_33 = inv_det*(m->_11 * m->_22 * m->_44 + m->_12 * m->_24 * m->_41 + m->_14 * m->_21 * m->_42 - m->_11 * m->_24 * m->_42 - m->_12 * m->_21 * m->_44 - m->_14 * m->_22 * m->_41);
		invm->_34 = inv_det*(m->_11 * m->_24 * m->_32 + m->_12 * m->_21 * m->_34 + m->_14 * m->_22 * m->_31 - m->_11 * m->_22 * m->_34 - m->_12 * m->_24 * m->_31 - m->_14 * m->_21 * m->_32);

		invm->_41 = inv_det*(m->_21 * m->_33 * m->_42 + m->_22 * m->_31 * m->_43 + m->_23 * m->_32 * m->_41 - m->_21 * m->_32 * m->_43 - m->_22 * m->_33 * m->_41 - m->_23 * m->_31 * m->_42);
		invm->_42 = inv_det*(m->_11 * m->_32 * m->_43 + m->_12 * m->_33 * m->_41 + m->_13 * m->_31 * m->_42 - m->_11 * m->_33 * m->_42 - m->_12 * m->_31 * m->_43 - m->_13 * m->_32 * m->_41);
		invm->_43 = inv_det*(m->_11 * m->_23 * m->_42 + m->_12 * m->_21 * m->_43 + m->_13 * m->_22 * m->_41 - m->_11 * m->_22 * m->_43 - m->_12 * m->_23 * m->_41 - m->_13 * m->_21 * m->_42);
		invm->_44 = inv_det*(m->_11 * m->_22 * m->_33 + m->_12 * m->_23 * m->_31 + m->_13 * m->_21 * m->_32 - m->_11 * m->_23 * m->_32 - m->_12 * m->_21 * m->_33 - m->_13 * m->_22 * m->_31);

		return true;
	}
}

void MatrixViewPort(MATRIX *mat){
	mat->_11 = WINDOW_WIDTH / 2; mat->_12 = 0.0f; mat->_13 = 0.0f; mat->_14 = 0.0f;
	mat->_21 = 0.0f; mat->_22 = -WINDOW_HEIGHT / 2; mat->_23 = 0.0f; mat->_24 = 0.0f;
	mat->_31 = 0.0f; mat->_32 = 0.0f; mat->_33 = 1.0f; mat->_34 = 0.0f;
	mat->_41 = WINDOW_WIDTH / 2; mat->_42 = WINDOW_HEIGHT / 2; mat->_43 = 0.0f; mat->_44 = 1.0f;
}

void Normalize(float *x, float *y, float *z, float *w) {

	float nor = (float)sqrt(*x * *x + *y * *y + *z * *z + *w * *w);
	if (nor != 0.0f) {
		*x = *x / nor;
		*y = *y / nor;
		*z = *z / nor;
		*w = *w / nor;
	}
	else {
		*x = 0.0f;
		*y = 0.0f;
		*z = 0.0f;
		*w = 0.0f;
	}
}

void Bdecode(char *bpass, char **binary, int *size){
	//暗号化時コード↓
	/*
	FILE *fp;
	FILE *fp2;
	char *binary;
	char *binary_change;
	int size;
	char fname[64];
	char fname_out[64];
	int i1;
	int len;
	int enemy_no = 0;
	int boss_no = 0;
	int bk = 0;

	while (1){
	scanf_s("%s", fname, 64);
	len = strlen(fname);
	if (len == 0 || len > 64)break;
	i1 = 0;
	do{
	strncpy(&fname_out[i1], &fname[i1], 1);
	i1++;
	} while (fname[i1] != '.');
	strncpy(&fname_out[i1++], ".", 1);
	strncpy(&fname_out[i1++], "d", 1);
	strncpy(&fname_out[i1++], "a", 1);
	strncpy(&fname_out[i1], "\0", 1);

	i1 = 0;
	if (strncmp(fname, "enemy.", 6) == 0)enemy_no = 1;
	if (strncmp(fname, "boss.", 5) == 0)boss_no = 1;

	do{
	if (enemy_no >= 1){
	sprintf(fname, "enemy%d.png", enemy_no);
	sprintf(fname_out, "enemy%d.da", enemy_no);
	}
	if (boss_no >= 1){
	sprintf(fname, "boss%d.png", boss_no);
	sprintf(fname_out, "boss%d.da", boss_no);
	}
	fp = NULL;
	fp2 = NULL;
	binary = NULL;
	binary_change = NULL;
	size = 0;

	fp = fopen(fname, "rb");
	if (fp == NULL){
	bk = 1; break;
	}

	while (!feof(fp)){
	size++; fgetc(fp);
	}
	size++;//終端文字含めた個数

	//ポインタを先頭に戻す
	fseek(fp, 0, SEEK_SET);

	binary = (char*)malloc(sizeof(char) * size);
	binary_change = (char*)malloc(sizeof(char) * size);

	for (int i = 0; i < size; i++){
	binary[i] = fgetc(fp);//終端文字まで読み込み
	}

	for (int i = 0; i < 100; i++){
	strncpy(&binary_change[i], &binary[99 - i], 1);
	}
	for (int i = 100; i < size; i++){
	strncpy(&binary_change[i], &binary[i], 1);
	}

	fp2 = fopen(fname_out, "wb");

	fwrite(binary_change, size, 1, fp2);

	free(binary);
	free(binary_change);
	fclose(fp);
	fclose(fp2);
	if (enemy_no >= 1)enemy_no++;
	if (boss_no >= 1)boss_no++;
	} while (enemy_no >= 1 || boss_no >= 1);
	enemy_no = boss_no = 0;
	if (bk == 1)break;
	}
	*/

	//復号コード↓
	FILE *fp = fopen(bpass, "rb");

	while (!feof(fp)){
		(*size)++; fgetc(fp);
	}
	(*size)++;//終端文字も含んだ個数

	//ポインタを先頭に戻す
	fseek(fp, 0, SEEK_SET);

	(*binary) = (char*)malloc(sizeof(char) * (*size));

	for (int i = 99; i >= 0; i--){
		(*binary)[i] = fgetc(fp);
	}
	for (int i = 100; i < (*size); i++){
		(*binary)[i] = fgetc(fp);
	}

	fclose(fp);
}