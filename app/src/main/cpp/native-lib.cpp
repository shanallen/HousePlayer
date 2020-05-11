#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <zconf.h>
#include "base/base.h"


extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <unistd.h>

}

extern "C" JNIEXPORT jstring JNICALL
Java_com_sjq_houseplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(av_version_info());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sjq_houseplayer_HousePlayer_nativeStart(JNIEnv *env, jobject instance, jstring path_,
                                                 jobject surface) {

    LOGE("-----准备执行了","ANativeWindow_fromSurface");
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env,surface);
    LOGE("-----执行了fromSurface","ANativeWindow_fromSurface");
    const char *path = env->GetStringUTFChars(path_,0);
    //ffmpeg视频绘制    音频绘制
    //初始化网络模块
    avformat_network_init();
    LOGE("-----执行了network_init","avformat_network_init");
    //总的Context
    AVFormatContext *formatContext = avformat_alloc_context();
    LOGE("-----执行了AVFormatContext","AVFormatContext");
    //
    AVDictionary *opts = NULL ;
    av_dict_set(&opts,"timeout","3000000",0);
    LOGE("-----执行了av_dict_set","av_dict_set");
    int ret = avformat_open_input(&formatContext,path,NULL,&opts);
    LOGE("-----执行打印了ret${ret}","av_dict_set");
    if(ret){
        LOGE("-----验证ret is return %d",ret);
        return;
    }
    LOGE("-----验证ret$ret");
    //视频流
    int vidio_stream_idx= -1;
    avformat_find_stream_info(formatContext,NULL);
    LOGE("-----准备执行了视频流的遍历","视频流的遍历");
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if(formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){

            vidio_stream_idx = i;
            break;
        }
    }
    LOGE("-----准备执行视频流索引","AVCodecParameters");
    //视频流索引
    AVCodecParameters *codecpar = formatContext->streams[vidio_stream_idx]->codecpar;
    LOGE("-----执行了视频流索引","AVCodecParameters");
    //解码器   h264  java 策略  key id
    //context.getLayoutInflater()   hashmap

    AVCodec *dec = avcodec_find_decoder(codecpar->codec_id);
    //解码器的上下文 ffmpeg 1.0  4.0
    AVCodecContext *codecContext = avcodec_alloc_context3(dec);
    //将解码器参数copy到解码器上下文
    avcodec_parameters_to_context(codecContext,codecpar);
    avcodec_open2(codecContext,dec,NULL);
    //解码yuv数据
    //AVPacket malloc()  newAVPacket
    LOGE("-----准备执行了AVPacket","AVPacket");
    AVPacket *packet = av_packet_alloc();
    LOGE("-----执行了AVPacket","AVPacket");
    //从视频流中读取数据包
    /**
     * SWS_FAST_BILINEAR     1
SWS_BILINEAR          2  重视速度
SWS_BICUBIC           4
SWS_X                 8
SWS_POINT          0x10
SWS_AREA           0x20
SWS_BICUBLIN       0x40
SWS_GAUSS          0x80
SWS_SINC          0x100
SWS_LANCZOS       0x200
SWS_SPLINE        0x400
     */
    SwsContext *swsContext = sws_getContext(codecContext->width,codecContext->height,codecContext->pix_fmt,codecContext->width,codecContext->height,AV_PIX_FMT_RGBA,SWS_BILINEAR,0,0,0);
    ANativeWindow_setBuffersGeometry(nativeWindow,codecContext->width,codecContext->height,WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer outBuffer;
    while (av_read_frame(formatContext,packet)>=0){

        avcodec_send_packet(codecContext,packet);
        //ffmpeg malloc()
        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(codecContext,frame);
        if(ret == AVERROR(EAGAIN)){
            continue;
        } else if(ret < 0){
            break;
        }

        //接受的容器
        uint8_t *dst_data[4];
        //每一行的首地址
        int dst_linesize[4];
        av_image_alloc(dst_data,dst_linesize,codecContext->width,codecContext->height,AV_PIX_FMT_RGBA,1);
        //绘制
        sws_scale(swsContext,frame->data,frame->linesize,0,frame->height,dst_data,dst_linesize);
        ANativeWindow_lock(nativeWindow,&outBuffer,NULL);
        LOGE("-----执行渲染之前","渲染");
        //渲染
        uint8_t *firstWindown = static_cast<uint8_t *>(outBuffer.bits);
        //输入源rgb的
        uint8_t *src_data = dst_data[0];
        //拿到一行有多少个字节RGBA
        int destStride = outBuffer.stride * 4;
        int src_linesize = dst_linesize[0];
        for (int i = 0; i <outBuffer.height ; ++i) {
            //内存拷贝，来进行渲染
            memcpy(firstWindown+i*destStride,src_data+i*src_linesize,destStride);
        }
        LOGE("-----执行ANativeWindow_unlockAndPost之前","ANativeWindow_unlockAndPost");
        ANativeWindow_unlockAndPost(nativeWindow);
        usleep(1000 * 16);
        av_frame_free(&frame);
        LOGE("-----执行av_frame_free之后","av_frame_free");
    }
    LOGE("-----执行了end","end");
    env->ReleaseStringUTFChars(path_,path);

}