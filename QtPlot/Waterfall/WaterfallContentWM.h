#pragma once

#include "Waterfall.h"

class WaterfallData;

class WaterfallContentWithMemory : public WaterfallContent
{
	Q_OBJECT

public:
	explicit WaterfallContentWithMemory(QCustomPlot* parent = nullptr);
	~WaterfallContentWithMemory() override;

public:
	void setInterval(int minval, int maxval) override;
	void append(double* data, int size, bool needUpdatePixmap) override;
	void setData(double* data, int width, int height) override;

	void setResolution(int width, int height) override;

	void clear() override;

private:
	WaterfallData* wfData;
		
};

class WaterfallData
{

public:
	void append(double* data, int width)
	{
		int currentOffset = _offset;
		if (_offset >= _height) currentOffset = _offset % _height;

		_dataIndexes[currentOffset] = _offset;
		std::memcpy(_data + (currentOffset * _width), data, sizeof(double) * _width);

		_offset++;
		_bIsSort = false;
	}

	void initialize(int inWidth, int inHeight)
	{
		_width = inWidth;
		_height = inHeight;

		delete _data;
		_data = new double[_width * _height];
		std::fill_n(_data, _width * _height, -10000);

		delete _data_temp;
		_data_temp = new double[_width];

		delete _dataIndexes;
		_dataIndexes = new int[_height];
		std::fill_n(_dataIndexes, _height, -1);

		_offset = 0;
	}

	void clear()
	{
		std::fill_n(_data, _width * _height, -10000);
		std::fill_n(_dataIndexes, _height, -1);
		_offset = 0;
	}

	void setData(double* data, int inWidth, int inHeight)
	{
		clear();
		std::memcpy(_data, data, sizeof(double) * inWidth * inHeight);
		_offset = inHeight;
		_bIsSort = true;
	}

	void sortData()
	{
		if (_bIsSort) return;

		quickSort(_dataIndexes, 0, _height - 1);

		_bIsSort = true;
	}

	inline double* data() const { return _data; }

	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int offset() const { return _offset; }

private:
	void quickSort(int arr[], int low, int high)
	{
		if (low >= high) return;

		int pivot = arr[high];
		int pos = partition(arr, low, high, pivot);

		quickSort(arr, low, pos - 1);
		quickSort(arr, pos + 1, high);
	}

	int partition(int arr[], int low, int high, int pivot)
	{
		int i = low;
		int j = low;
		while(i <= high)
		{
			if(arr[i] > pivot)
			{
				i++;
			}
			else
			{
				swap(arr, i, j);
				i++;
				j++;
			}
		}
		return j - 1;
	}

	void swap(int arr[], int pos1, int pos2)
	{
		int temp = arr[pos1];
		arr[pos1] = arr[pos2];
		arr[pos2] = temp;

		memcpy(_data_temp, _data + pos1 * _width, sizeof(double) * _width);
		memcpy(_data + pos1 * _width, _data + pos2 * _width, sizeof(double) * _width);
		memcpy(_data + pos2 * _width, _data_temp, sizeof(double) * _width);
	}

private:
	double* _data = nullptr;
	double* _data_temp = nullptr;
	int* _dataIndexes = nullptr;
	
	int _offset = 0;

	int _width = 0;
	int _height = 0;

	bool _bIsSort = false;

};


