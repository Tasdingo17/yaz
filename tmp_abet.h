// File for available bandwidht estimator base class.

#ifndef __ABET_H__
#define __ABET_H__

#include <memory>
#include <sys/time.h>
#include <list>
#include <vector>

struct MeasurementBundle;

class ABSender{
public:
    virtual void run() = 0;
    virtual std::unique_ptr<ABSender> clone() const = 0;
    virtual bool validate() = 0;
    virtual void setupRun() = 0;
    virtual void cleanup() = 0;
    virtual bool doOneMeasurementRound(std::list<MeasurementBundle> *) = 0;
    virtual bool processOneRoundRes(std::list<MeasurementBundle> *) = 0;
    virtual void resetRound() = 0;
    virtual float get_current_estimation() const = 0;
    virtual ~ABSender(){};
};


class ABReceiver{
public:
    virtual void run() = 0;
    virtual std::unique_ptr<ABReceiver> clone() const = 0;
    virtual bool validate() = 0;
    virtual void cleanup() = 0;
    virtual ~ABReceiver(){};
};


struct MeasurementBundle
{
    MeasurementBundle() : m_local_app_mean(0), 
                            m_local_pcap_mean(0), 
                            m_remote_app_mean(0), 
                            m_remote_pcap_mean(0), 
                            m_local_ttl(0), m_remote_ttl(0),
                            m_local_nsamples(0), m_local_nlost(0),
                            m_remote_nsamples(0), m_remote_nlost(0)
        {
            timerclear(&m_start);
            timerclear(&m_end);
        }

    bool operator==(const MeasurementBundle &mb)
        {
            return (this->m_start.tv_sec == mb.m_start.tv_sec &&
                    this->m_start.tv_usec == mb.m_start.tv_usec);
        }

    bool operator<(const MeasurementBundle &mb)
        {
            return (timercmp(&this->m_start, &mb.m_start, <));
        }

    void reset()
        {
            timerclear(&m_start);
            timerclear(&m_end);

            m_local_app_mean =
                m_local_pcap_mean =
                m_remote_app_mean =
                m_remote_pcap_mean = 0.0;

            m_local_ttl = m_remote_ttl = 0;

            m_local_nsamples =
                m_local_nlost =
                m_remote_nsamples =
                m_remote_nlost = 0;

            m_delays_vec.clear();
        }

    struct timeval m_start;
    struct timeval m_end;

    float m_local_app_mean;
    float m_local_pcap_mean;

    float m_remote_app_mean;
    float m_remote_pcap_mean;

    unsigned int m_local_ttl;
    unsigned int m_remote_ttl;

    unsigned int m_local_nsamples;
    unsigned int m_local_nlost;
    unsigned int m_remote_nsamples;
    unsigned int m_remote_nlost;

    std::vector<timeval> m_delays_vec;
};


#endif