#include "config.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>

#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER
# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include "cmdutils.h"

const char program_name[] = "ffmpeg_principle";
const int program_birth_year = 2003;

void show_help_default(const char *opt, const char *arg)
{
}

static const OptionDef options[] = {
    CMDUTILS_COMMON_OPTIONS
};

// FFmpeg打开输入文件
// https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/input.html

int avformat_open_input_example()
{
    av_log(NULL, AV_LOG_ERROR, "----- avformat_open_input_example ------ \n");

    AVFormatContext *fmt_ctx = NULL;
    int type = 1;
    int err;
    char filename[] = "juren-5s.mp4";

    fmt_ctx = avformat_alloc_context();
    if(!fmt_ctx)
    {
        av_log(NULL, AV_LOG_INFO, "error code %d \n", AVERROR(ENOMEM));
        return 1;
    }

    if(1 == type)
    {
        if((err = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0)
        {
            av_log(NULL, AV_LOG_INFO, "error code %d \n", err);
        }
        else
        {
            av_log(NULL, AV_LOG_INFO, "open success \n");
            av_log(NULL, AV_LOG_INFO, "url - %s \n", fmt_ctx->url);
            av_log(NULL, AV_LOG_INFO, "duration - %I64d \n", fmt_ctx->duration);
            av_log(NULL, AV_LOG_INFO, "nb_streams - %u \n", fmt_ctx->nb_streams);
            for(int i = 0; i < fmt_ctx->nb_streams; i++)
            {
                av_log(NULL, AV_LOG_INFO, "stream codec_type - %d \n", fmt_ctx->streams[i]->codecpar->codec_type);
            }
            av_log(NULL, AV_LOG_INFO, "iformat name - %s \n", fmt_ctx->iformat->name);
            av_log(NULL, AV_LOG_INFO, "iformat long name - %s \n", fmt_ctx->iformat->long_name);
        }

    }

    if(2 == type)
    {
        // 设置探测大小
        AVDictionary *format_opts = NULL;
        av_dict_set(&format_opts, "probesize", "32", 0);

        if((err = avformat_open_input(&fmt_ctx, filename, NULL, &format_opts)) < 0)
        {
            av_log(NULL, AV_LOG_INFO, "error code %d \n", err);
        }
        else
        {
            avformat_find_stream_info(fmt_ctx, NULL);
            av_log(NULL, AV_LOG_INFO, "open success \n");
            av_log(NULL, AV_LOG_INFO, "url - %s \n", fmt_ctx->url);
            av_log(NULL, AV_LOG_INFO, "duration - %I64d \n", fmt_ctx->duration);
            av_log(NULL, AV_LOG_INFO, "nb_streams - %u \n", fmt_ctx->nb_streams);
            for(int i = 0; i < fmt_ctx->nb_streams; i++)
            {
                av_log(NULL, AV_LOG_INFO, "stream codec_type - %d \n", fmt_ctx->streams[i]->codecpar->codec_type);
            }
            av_log(NULL, AV_LOG_INFO, "iformat name - %s \n", fmt_ctx->iformat->name);
            av_log(NULL, AV_LOG_INFO, "iformat long name - %s \n", fmt_ctx->iformat->long_name);
        }
        av_dict_free(&format_opts);
    }

    return 0;
}

// FFmpeg读取文件内容AVPacket
// https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/avpacket.html

int AVPacket_example()
{
    av_log(NULL, AV_LOG_ERROR, "----- AVPacket_example ------ \n");

    AVFormatContext *fmt_ctx = NULL;
    int type = 2;

    int err;
    char filename[] = "juren-5s.mp4";

    fmt_ctx = avformat_alloc_context();
    if(!fmt_ctx)
    {
        av_log(NULL, AV_LOG_INFO, "error code %d \n", AVERROR(ENOMEM));
        return ENOMEM;
    }

    if((err = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_INFO, "error code %d \n", err);
        return err;
    }

    if(1 == type)
    {
        AVPacket *pkt = av_packet_alloc();
        int ret = 0;
        ret = av_read_frame(fmt_ctx, pkt);
        if(ret < 0)
        {
            av_log(NULL, AV_LOG_INFO, "read fail \n");
            return ret;
        }
        else
        {
            av_log(NULL, AV_LOG_INFO, "read success \n");
            av_log(NULL, AV_LOG_INFO, "stream 0 type : %d \n", fmt_ctx->streams[0]->codecpar->codec_type);
            av_log(NULL, AV_LOG_INFO, "stream 1 type : %d \n", fmt_ctx->streams[1]->codecpar->codec_type);
            av_log(NULL, AV_LOG_INFO, "stream_index : %d \n", pkt->stream_index);
            av_log(NULL, AV_LOG_INFO, "duration : %I64d ,time_base : %d/%d \n", pkt->duration,
                fmt_ctx->streams[1]->time_base.num, fmt_ctx->streams[0]->time_base.den);
            av_log(NULL, AV_LOG_INFO, "size : %d \n", pkt->size);
            av_log(NULL, AV_LOG_INFO, "pos : %I64d \n", pkt->pos);
            av_log(NULL, AV_LOG_INFO, "data : %x %x %x %x %x %x %x \n",
                pkt->data[0], pkt->data[1], pkt->data[2], pkt->data[3], pkt->data[4],
                pkt->data[5], pkt->data[6]);
        }
        av_packet_free(&pkt);
    }

    if(2 == type)
    {
        av_log(NULL, AV_LOG_INFO, "stream numbers : %d \n", fmt_ctx->nb_streams);

        AVPacket *pkt = av_packet_alloc();
        int ret = 0, i;
        for(i = 0; i < 100; i++)
        {
            ret = av_read_frame(fmt_ctx, pkt);
            if(ret < 0)
            {
                av_log(NULL, AV_LOG_INFO, "read fail \n");
                return ret;
            }
            else
            {
                enum AVMediaType type = fmt_ctx->streams[pkt->stream_index]->codecpar->codec_type;
                if(type == AVMEDIA_TYPE_VIDEO)
                {
                    av_log(NULL, AV_LOG_INFO, "-------- stream video ------- \n");
                }
                else if(type == AVMEDIA_TYPE_AUDIO)
                {
                    av_log(NULL, AV_LOG_INFO, "-------- stream audio ------- \n");
                }

                av_log(NULL, AV_LOG_INFO, "duration : %I64d ,time_base : %d/%d \n", pkt->duration,
                    fmt_ctx->streams[pkt->stream_index]->time_base.num, fmt_ctx->streams[pkt->stream_index]->time_base.den);
                av_log(NULL, AV_LOG_INFO, "size : %d \n", pkt->size);
                av_log(NULL, AV_LOG_INFO, "pos : %I64d \n", pkt->pos);
                av_log(NULL, AV_LOG_INFO, "data : %x %x %x %x %x %x %x \n",
                    pkt->data[0], pkt->data[1], pkt->data[2], pkt->data[3], pkt->data[4],
                    pkt->data[5], pkt->data[6]);

                av_packet_unref(pkt);
            }
        }
        av_packet_free(&pkt);
    }

    return 0;
}


// 如何设置解复用器参数
// https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/demuxer_args.html

int AVDictionary_example()
{
    av_log(NULL, AV_LOG_ERROR, "----- AVDictionary_example ------ \n");

    AVFormatContext *fmt_ctx = NULL;
    int err;
    char filename[] = "juren-5s.mp4";

    fmt_ctx = avformat_alloc_context();
    if(!fmt_ctx)
    {
        av_log(NULL, AV_LOG_ERROR, "error code %d \n", AVERROR(ENOMEM));
        return 1;
    }

    AVDictionary* format_opts = NULL;
    AVDictionaryEntry *t;
    av_dict_set(&format_opts, "formatprobesize", "10485760", AV_DICT_MATCH_CASE);
    av_dict_set(&format_opts, "export_all", "1", AV_DICT_MATCH_CASE);
    av_dict_set(&format_opts, "export_666", "1", AV_DICT_MATCH_CASE);

    // 获取字典里的第一个属性
    if((t = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX)))
    {
        av_log(NULL, AV_LOG_INFO, "Option key: %s , value %s \n", t->key, t->value);
    }
    if((err = avformat_open_input(&fmt_ctx, filename, NULL, &format_opts)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "error code %d \n", err);
    }
    else
    {
        av_log(NULL, AV_LOG_INFO, "open success \n");
        av_log(NULL, AV_LOG_INFO, "duration: %I64d \n", fmt_ctx->duration);
    }
    // 再次，获取字典里的第一个属性
    if((t = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX)))
    {
        av_log(NULL, AV_LOG_INFO, "Option key: %s , value %s \n", t->key, t->value);
    }

    av_dict_free(&format_opts);

    return 0;
}

/* Called from the main */
int main(int argc, char **argv)
{
    av_log_set_level(AV_LOG_INFO);
    av_log_set_flags(AV_LOG_SKIP_REPEATED);

    parse_loglevel(argc, argv, options);

    av_log(NULL, AV_LOG_INFO, "start ffmpeg principle\n");

    avformat_open_input_example();

    AVPacket_example();

    AVDictionary_example();

    return 0;
}
