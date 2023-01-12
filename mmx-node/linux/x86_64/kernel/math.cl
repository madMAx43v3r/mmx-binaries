
float square_norm_2(const float2 a) {
	return (a.x*a.x + a.y*a.y);
}

float square_norm_3(const float3 a) {
	return (a.x*a.x + a.y*a.y + a.z*a.z);
}

float square_norm_4(const float4 a) {
	return (a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
}

float2 mul_22_2(const float* mat, const float2 b) {
	float2 res;
	res.x = mat[0] * b.x + mat[2] * b.y;
	res.y = mat[1] * b.x + mat[3] * b.y;
	return res;
}

float3 mul_33_3(const float* mat, const float3 b) {
	float3 res;
	res.x = mat[0] * b.x + mat[3] * b.y + mat[6] * b.z;
	res.y = mat[1] * b.x + mat[4] * b.y + mat[7] * b.z;
	res.z = mat[2] * b.x + mat[5] * b.y + mat[8] * b.z;
	return res;
}

float3 mul_34_3(const float* mat, const float3 b) {
	float3 res;
	res.x = mat[0] * b.x + mat[3] * b.y + mat[6] * b.z + mat[9];
	res.y = mat[1] * b.x + mat[4] * b.y + mat[7] * b.z + mat[10];
	res.z = mat[2] * b.x + mat[5] * b.y + mat[8] * b.z + mat[11];
	return res;
}

float2 gmul_22_2(__global const float* mat, const float2 b) {
	float2 res;
	res.x = mat[0] * b.x + mat[2] * b.y;
	res.y = mat[1] * b.x + mat[3] * b.y;
	return res;
}

float3 gmul_33_3(__global const float* mat, const float3 b) {
	float3 res;
	res.x = mat[0] * b.x + mat[3] * b.y + mat[6] * b.z;
	res.y = mat[1] * b.x + mat[4] * b.y + mat[7] * b.z;
	res.z = mat[2] * b.x + mat[5] * b.y + mat[8] * b.z;
	return res;
}

float3 gmul_34_3(__global const float* mat, const float3 b) {
	float3 res;
	res.x = mat[0] * b.x + mat[3] * b.y + mat[6] * b.z + mat[9];
	res.y = mat[1] * b.x + mat[4] * b.y + mat[7] * b.z + mat[10];
	res.z = mat[2] * b.x + mat[5] * b.y + mat[8] * b.z + mat[11];
	return res;
}

/*
 * Matrix multiplication assuming row major storage: Y_NK = A_NM * B_MK
 */
void mul_NM_K(int N, int M, int K, float* Y, const float* A, const float* B) {
	for(int i = 0; i < N*K; ++i) {
		Y[i] = 0;
	}
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < K; ++j) {
			for(int k = 0; k < M; ++k) {
				Y[j*N + i] += A[k*N + i] * B[j*M + k];
			}
		}
	}
}

/*
 * Matrix multiplication assuming row major storage: Y_MK = A_NM^T * B_MK
 */
void mul_NM_T_K(int N, int M, int K, float* Y, const float* A, const float* B) {
	for(int i = 0; i < M*K; ++i) {
		Y[i] = 0;
	}
	for(int i = 0; i < M; ++i) {
		for(int j = 0; j < K; ++j) {
			for(int k = 0; k < N; ++k) {
				Y[j*M + i] += A[i*N + k] * B[j*N + k];
			}
		}
	}
}

void get_rotate2(float* mat, const float radians) {
	mat[0] = cos(radians);
	mat[1] = sin(radians);
	mat[2] = -sin(radians);
	mat[3] = cos(radians);
}

float2 vec_rotate2(const float2 b, const float radians) {
	float mat[4];
	get_rotate2(mat, radians);
	return mul_22_2(mat, b);
}

void transform2(float* mat, const float3 pose) {
	mat[0 + 0 * 3] = cos(pose.z);
	mat[1 + 0 * 3] = sin(pose.z);
	mat[2 + 0 * 3] = 0;
	mat[0 + 1 * 3] = -sin(pose.z);
	mat[1 + 1 * 3] = cos(pose.z);
	mat[2 + 1 * 3] = 0;
	mat[0 + 2 * 3] = pose.x;
	mat[1 + 2 * 3] = pose.y;
	mat[2 + 2 * 3] = 1;
}

void inverse_33(float* A_inv, const float* A) {
	const float det = 	A[0 + 0 * 3] * (A[1 + 1 * 3] * A[2 + 2 * 3] - A[2 + 1 * 3] * A[1 + 2 * 3]) -
						A[0 + 1 * 3] * (A[1 + 0 * 3] * A[2 + 2 * 3] - A[1 + 2 * 3] * A[2 + 0 * 3]) +
						A[0 + 2 * 3] * (A[1 + 0 * 3] * A[2 + 1 * 3] - A[1 + 1 * 3] * A[2 + 0 * 3]);
	const float inv_det = 1.f / det;
	A_inv[0 + 0 * 3] = (A[1 + 1 * 3] * A[2 + 2 * 3] - A[2 + 1 * 3] * A[1 + 2 * 3]) * inv_det;
	A_inv[0 + 1 * 3] = (A[0 + 2 * 3] * A[2 + 1 * 3] - A[0 + 1 * 3] * A[2 + 2 * 3]) * inv_det;
	A_inv[0 + 2 * 3] = (A[0 + 1 * 3] * A[1 + 2 * 3] - A[0 + 2 * 3] * A[1 + 1 * 3]) * inv_det;
	A_inv[1 + 0 * 3] = (A[1 + 2 * 3] * A[2 + 0 * 3] - A[1 + 0 * 3] * A[2 + 2 * 3]) * inv_det;
	A_inv[1 + 1 * 3] = (A[0 + 0 * 3] * A[2 + 2 * 3] - A[0 + 2 * 3] * A[2 + 0 * 3]) * inv_det;
	A_inv[1 + 2 * 3] = (A[1 + 0 * 3] * A[0 + 2 * 3] - A[0 + 0 * 3] * A[1 + 2 * 3]) * inv_det;
	A_inv[2 + 0 * 3] = (A[1 + 0 * 3] * A[2 + 1 * 3] - A[2 + 0 * 3] * A[1 + 1 * 3]) * inv_det;
	A_inv[2 + 1 * 3] = (A[2 + 0 * 3] * A[0 + 1 * 3] - A[0 + 0 * 3] * A[2 + 1 * 3]) * inv_det;
	A_inv[2 + 2 * 3] = (A[0 + 0 * 3] * A[1 + 1 * 3] - A[1 + 0 * 3] * A[0 + 1 * 3]) * inv_det;
}


