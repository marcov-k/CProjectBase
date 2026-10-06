#include "../include/m_vector.h"

#include "../include/m_result.h"
#include "../include/m_contain.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int GrowVector(Vector* v, size_t length)
{
	if (v == NULL || v->itemSize == 0) return M_FAILURE_GEN;
	if (length == 0) return M_SUCCESS;
	if (v->data != NULL && v->capacity >= length) return M_SUCCESS;

	size_t capacity = max(v->capacity, 1);
	while (capacity < length)
	{
		if (capacity > SIZE_MAX / 2) return M_FAILURE_GEN;
		capacity *= 2;
	}

	if (capacity > SIZE_MAX / v->itemSize) return M_FAILURE_GEN;
	void* newBuffer = realloc(v->data, capacity * v->itemSize);
	if (newBuffer == NULL) return M_FAILURE_GEN;

	v->data = newBuffer;
	v->capacity = capacity;

	return M_SUCCESS;
}

Vector CreateVector(const void* data, size_t length, size_t itemSize, Destructor destructor, Copier copier)
{
	Vector v = { NULL, destructor, itemSize, 0, 0 };

	if (length == 0 || itemSize == 0) return v;

	if (GrowVector(&v, length) != M_SUCCESS) goto FAILURE;

	if (data != NULL)
	{
		if (copier == NULL)
		{
			memcpy(v.data, data, length * itemSize);
			v.length = length;
		}
		else
		{
			for (size_t i = 0; i < length; ++i)
			{
				size_t dataIndex = i * itemSize;
				void* vPtr = (char*)v.data + dataIndex;
				void* dataPtr = (char*)data + dataIndex;
				if (copier(vPtr, dataPtr) != M_SUCCESS) goto FAILURE;
			}
		}
	}

	return v;

FAILURE:
	v.itemSize = 0;
	return v;
}

int DestroyVector(void* v)
{
	if (v == NULL) return M_FAILURE_GEN;

	Vector* vec = (Vector*)v;

	if (vec->itemSize == 0) return M_FAILURE_GEN;

	if (vec->destructor != NULL)
	{
		for (size_t i = 0; i < vec->length; ++i)
		{
			if (vec->destructor((char*)vec->data + i * vec->itemSize) != M_SUCCESS) return M_FAILURE_GEN;
		}
	}

	free(vec->data);
	vec->data = NULL;
	vec->destructor = NULL;
	vec->itemSize = 0;
	vec->length = 0;
	vec->capacity = 0;

	return M_SUCCESS;
}

int ClearVector(Vector* v)
{
	if (v == NULL || v->itemSize == 0) return M_FAILURE_GEN;

	if (v->destructor != NULL)
	{
		for (size_t i = 0; i < v->length; ++i)
		{
			if (v->destructor((char*)v->data + i * v->itemSize) != M_SUCCESS) return M_FAILURE_GEN;
		}
	}
	v->length = 0;

	return M_SUCCESS;
}

int MoveVector(Vector* dest, Vector* source)
{
	if (dest == NULL || source == NULL || source->itemSize == 0) return M_FAILURE_GEN;
	if (dest == source) return M_SUCCESS;

	dest->itemSize = source->itemSize;

	if (GrowVector(dest, source->length) != M_SUCCESS) goto FAILURE;

	memcpy(dest->data, source->data, source->length * source->itemSize);
	dest->destructor = source->destructor;
	dest->length = source->length;

	source->destructor = NULL;
	DestroyVector(source); // cannot fail (source != NULL; source->itemSize != 0)

	return M_SUCCESS;

FAILURE:
	dest->itemSize = 0;
	return M_FAILURE_GEN;
}

Vector CopyVector(const Vector* v, Copier copier)
{
	Vector copy = { NULL, NULL, 0, 0, 0 };

	if (v == NULL) return copy;

	copy.itemSize = v->itemSize;

	if (GrowVector(&copy, v->length) != M_SUCCESS) goto FAILURE;

	if (copier == NULL)
	{
		memcpy(copy.data, v->data, v->length * v->itemSize);
	}
	else
	{
		for (size_t i = 0; i < v->length; ++i)
		{
			size_t dataIndex = i * v->itemSize;
			void* copyPtr = (char*)copy.data + dataIndex;
			void* sourcePtr = (char*)v->data + dataIndex;
			if (copier(copyPtr, sourcePtr) != M_SUCCESS) goto FAILURE;
		}
	}
	copy.destructor = v->destructor;
	copy.length = v->length;

	goto SUCCESS;

FAILURE:
	copy.itemSize = 0;

SUCCESS:
	return copy;
}

int CopyVectorTo(void* dest, const void* source, Copier copier)
{
	if (dest == NULL || source == NULL) return M_FAILURE_GEN;

	Vector* destVec = (Vector*)dest;
	Vector* sourceVec = (Vector*)source;

	if (sourceVec->itemSize == 0) return M_FAILURE_GEN;

	if (destVec == sourceVec) return M_SUCCESS;

	destVec->itemSize = sourceVec->itemSize;
	destVec->destructor = sourceVec->destructor;

	if (sourceVec->length == 0)
	{
		if (ClearVector(destVec) != M_SUCCESS) goto FAILURE;
		return M_SUCCESS;
	}

	if (GrowVector(destVec, sourceVec->length) != M_SUCCESS) goto FAILURE;

	if (copier == NULL)
	{
		memcpy(destVec->data, sourceVec->data, sourceVec->length * sourceVec->itemSize);
	}
	else
	{
		for (size_t i = 0; i < sourceVec->length; ++i)
		{
			size_t dataIndex = i * sourceVec->itemSize;
			void* destPtr = (char*)destVec->data + dataIndex;
			void* sourcePtr = (char*)sourceVec->data + dataIndex;
			if (copier(destPtr, sourcePtr) != M_SUCCESS) goto FAILURE;
		}
	}
	destVec->length = sourceVec->length;

	return M_SUCCESS;

FAILURE:
	destVec->itemSize = 0;
	return M_FAILURE_GEN;
}

int CopyDataToVector(Vector* dest, const void* source, size_t sourceLen, Copier copier)
{
	if (dest == NULL) return M_FAILURE_GEN;

	if (source == NULL || sourceLen == 0)
	{
		if (ClearVector(dest) != M_SUCCESS) goto FAILURE;
		return M_SUCCESS;
	}

	if (GrowVector(dest, sourceLen) != M_SUCCESS) goto FAILURE;

	if (copier == NULL)
	{
		memcpy(dest->data, source, sourceLen * dest->itemSize);
	}
	else
	{
		for (size_t i = 0; i < sourceLen; ++i)
		{
			size_t dataIndex = i * dest->itemSize;
			void* destPtr = (char*)dest->data + dataIndex;
			void* sourcePtr = (char*)source + dataIndex;
			if (copier(destPtr, sourcePtr) != M_SUCCESS) goto FAILURE;
		}
	}
	dest->length = sourceLen;

	return M_SUCCESS;

FAILURE:
	dest->itemSize = 0;
	return M_FAILURE_GEN;
}

void* GetElementAt(const Vector* v, size_t index)
{
	if (v == NULL || v->itemSize == 0) return NULL;
	if (index >= v->length) return NULL;

	return (char*)v->data + index * v->itemSize;
}

int SetElementAt(Vector* v, size_t index, const void* item, Copier copier)
{
	if (v == NULL || v->itemSize == 0 || item == NULL) return M_FAILURE_GEN;
	if (index >= v->length) return M_FAILURE_GEN;

	void* vPtr = (char*)v->data + index * v->itemSize;
	if (copier == NULL)
	{
		memcpy(vPtr, item, v->itemSize);
	}
	else
	{
		if (copier(vPtr, item) != M_SUCCESS) return M_FAILURE_GEN;
	}

	return M_SUCCESS;
}