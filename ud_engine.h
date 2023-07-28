#ifndef UD_ENGINE_H
#define UD_ENGINE_H


/*****************************************************************************
* @author : LeonardoLee
* @date : 2023/5/20
* @last change :
* @description : TensorRT Engine
*****************************************************************************/
#pragma once
#include <queue>	// ϵͳ��

#include <cuda_runtime.h>  // cuda��
#include <cuda_runtime_api.h>

#include <opencv2/opencv.hpp> // opencv��

#include "NvInfer.h"  //TensorRT��
#include "NvOnnxParser.h"
#include <condition_variable>
#include "common.h"  // TensorRT samples�еĺ���
#include "buffers.h"
#include "logging.h"

#include "ud_basetype.h"	// �Զ�������ݽṹ�� ����Ľӿ�
#include "ud_loguru.hpp" // https://github.com/emilk/loguru


// \! ����ָ��, ��������TRT�����м����͵Ķ�ռ����ָ��
template <typename T>
using SampleUniquePtr = std::unique_ptr<T, samplesCommon::InferDeleter>;



// \! ------------------------------------Engine Start------------------------------
class CTrtEngine {
public:
	CTrtEngine() {};

	// \!���湹��
	// \@param ����PARAMS_S����
	// \@param �������nErrorFlag
	CTrtEngine(const PARAMS_S& params, PD_S32& nErrnoFlag);

	std::shared_ptr<nvinfer1::ICudaEngine> Get() const;

public:
	// \! �����������
	std::vector<nvinfer1::Dims> mInputDims;	                    // The dimensions of the input to the network.
	std::vector<nvinfer1::Dims> mOutputDims;                     // The dimensions of the output to the network.
	std::vector<std::string> mInputTensorNames;	    // ģ�����������
	std::vector<std::string> mOutputTensorNames;	// ģ�����������

private:
	PD_S32 buildONNX(const PARAMS_S& params);			// Build Engine
	PD_S32 loadEngine(const PARAMS_S& params);			// ����Engine

private:
	std::shared_ptr<nvinfer1::ICudaEngine> mEngine; //!< The TensorRT engine used to run the network

};
// \! ------------------------------------Engine End------------------------------


// \! ----------------------Context ִ���������̳߳ز���----------------------------

// \! A simple threadsafe queue using a mutex and a condition variable.
template <typename T>
class Queue
{
public:
	Queue() = default;

	Queue(Queue&& other)
	{
		std::unique_lock<std::mutex> lock(other.mutex_);
		queue_ = std::move(other.queue_);
	}

	PD_VOID Push(T value)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		queue_.push(std::move(value));
		lock.unlock();
		cond_.notify_one();
	}

	T Pop()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this] {return !queue_.empty(); });
		T value = std::move(queue_.front());
		queue_.pop();
		return value;
	}

	size_t Size()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		return queue_.size();
	}

private:
	mutable std::mutex mutex_;
	std::queue<T> queue_;
	std::condition_variable cond_;
};

// \! A pool of available contexts is simply implemented as a queue for our example. 
template <typename Context>
using ContextPool = Queue<std::unique_ptr<Context>>;

// \! A RAII class for acquiring an execution context from a context pool. 
template <typename Context>
class ScopedContext
{
public:
	explicit ScopedContext(ContextPool<Context>& pool)
		: pool_(pool), context_(pool_.Pop())
	{
		context_->Activate();
	}

	~ScopedContext()
	{
		context_->Deactivate();
		pool_.Push(std::move(context_));
	}

	Context* operator->() const
	{
		return context_.get();
	}

private:
	ContextPool<Context>& pool_;
	std::unique_ptr<Context> context_;
};

// \! ִ��������
class ExecContext
{
public:
	friend ScopedContext<ExecContext>;

	ExecContext(PD_S32 device, std::shared_ptr<nvinfer1::ICudaEngine> engine) : device_(device)
	{
		cudaError_t st = cudaSetDevice(device_);

		if (st != cudaSuccess)
			throw std::invalid_argument("could not set CUDA device");

		context_.reset(engine->createExecutionContext());
	}

	nvinfer1::IExecutionContext* getContext()
	{
		return context_.get();
	}

private:
	PD_VOID Activate() {}
	PD_VOID Deactivate() {}
private:
	PD_S32 device_;
	SampleUniquePtr<nvinfer1::IExecutionContext> context_;
};
// \! ----------------------Context ִ���������̳߳ز��� END---------------------------


// \! ִ�������Ľṹ��
struct TRTCore_ctx
{
	PARAMS_S params;							// \! ִ�������ĵ�Engine
	std::shared_ptr<CTrtEngine> engine;		// \! һ��ģ�Ͷ�Ӧһ��Engine
	ContextPool<ExecContext> pool;          // \! һ��Engine ��Ӧ���ģ��
};

#endif // !UD_ENGINE_H