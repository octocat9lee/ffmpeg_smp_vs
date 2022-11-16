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

// FFmpeg读取文件内容AVPacket
// https://ffmpeg.xianwaizhiyin.net/api-ffmpeg/avpacket.html

int avpacket_example()
{
    AVFormatContext *fmt_ctx = NULL;
    int type = 2;

    int err;
    char filename[] = "juren-5s.mp4";

    fmt_ctx = avformat_alloc_context();
    if(!fmt_ctx)
    {
        printf("error code %d \n", AVERROR(ENOMEM));
        return ENOMEM;
    }

    if((err = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0)
    {
        printf("error code %d \n", err);
        return err;
    }

    if(1 == type)
    {
        AVPacket *pkt = av_packet_alloc();
        int ret = 0;
        ret = av_read_frame(fmt_ctx, pkt);
        if(ret < 0)
        {
            printf("read fail \n");
            return ret;
        }
        else
        {
            printf("read success \n");
            printf("stream 0 type : %d \n", fmt_ctx->streams[0]->codecpar->codec_type);
            printf("stream 1 type : %d \n", fmt_ctx->streams[1]->codecpar->codec_type);
            printf("stream_index : %d \n", pkt->stream_index);
            printf("duration : %I64d ,time_base : %d/%d \n", pkt->duration,
                fmt_ctx->streams[1]->time_base.num, fmt_ctx->streams[0]->time_base.den);
            printf("size : %d \n", pkt->size);
            printf("pos : %I64d \n", pkt->pos);
            printf("data : %x %x %x %x %x %x %x \n",
                pkt->data[0], pkt->data[1], pkt->data[2], pkt->data[3], pkt->data[4],
                pkt->data[5], pkt->data[6]);
        }
        av_packet_free(&pkt);
    }

    if(2 == type)
    {
        printf("stream numbers : %d \n", fmt_ctx->nb_streams);

        AVPacket *pkt = av_packet_alloc();
        int ret = 0, i;
        for(i = 0; i < 100; i++)
        {
            ret = av_read_frame(fmt_ctx, pkt);
            if(ret < 0)
            {
                printf("read fail \n");
                return ret;
            }
            else
            {
                enum AVMediaType type = fmt_ctx->streams[pkt->stream_index]->codecpar->codec_type;
                if(type == AVMEDIA_TYPE_VIDEO)
                {
                    printf("-------- stream video ------- \n");
                }
                else if(type == AVMEDIA_TYPE_AUDIO)
                {
                    printf("-------- stream audio ------- \n");
                }

                printf("duration : %I64d ,time_base : %d/%d \n", pkt->duration,
                    fmt_ctx->streams[pkt->stream_index]->time_base.num, fmt_ctx->streams[pkt->stream_index]->time_base.den);
                printf("size : %d \n", pkt->size);
                printf("pos : %I64d \n", pkt->pos);
                printf("data : %x %x %x %x %x %x %x \n",
                    pkt->data[0], pkt->data[1], pkt->data[2], pkt->data[3], pkt->data[4],
                    pkt->data[5], pkt->data[6]);

                av_packet_unref(pkt);
            }
        }
        av_packet_free(&pkt);
    }
}

/* Called from the main */
int main(int argc, char **argv)
{
    av_log_set_level(AV_LOG_ERROR);
    av_log_set_flags(AV_LOG_SKIP_REPEATED);

    parse_loglevel(argc, argv, options);

    av_log(NULL, AV_LOG_ERROR, "start ffmpeg principle\n");

    avpacket_example();

    return 0;
}
