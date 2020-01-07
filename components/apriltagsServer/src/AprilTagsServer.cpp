//
// Copyright (c) ZeroC, Inc. All rights reserved.
//
//
// Ice version 3.7.3
//
// <auto-generated>
//
// Generated from file `AprilTagsServer.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <AprilTagsServer.h>
#include <IceUtil/PushDisableWarnings.h>
#include <Ice/LocalException.h>
#include <Ice/ValueFactory.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/InputStream.h>
#include <Ice/OutputStream.h>
#include <IceUtil/PopDisableWarnings.h>

#if defined(_MSC_VER)
#   pragma warning(disable:4458) // declaration of ... hides class member
#elif defined(__clang__)
#   pragma clang diagnostic ignored "-Wshadow"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wshadow"
#endif

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 307
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 >= 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 3
#       error Ice patch level mismatch!
#   endif
#endif

#ifdef ICE_CPP11_MAPPING // C++11 mapping

namespace
{

const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[2] =
{
    "::Ice::Object",
    "::RoboCompAprilTagsServer::AprilTagsServer"
};
const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_ops[] =
{
    "getAprilTags",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};
const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name = "getAprilTags";

}

bool
RoboCompAprilTagsServer::AprilTagsServer::ice_isA(::std::string s, const ::Ice::Current&) const
{
    return ::std::binary_search(iceC_RoboCompAprilTagsServer_AprilTagsServer_ids, iceC_RoboCompAprilTagsServer_AprilTagsServer_ids + 2, s);
}

::std::vector<::std::string>
RoboCompAprilTagsServer::AprilTagsServer::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector<::std::string>(&iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[0], &iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[2]);
}

::std::string
RoboCompAprilTagsServer::AprilTagsServer::ice_id(const ::Ice::Current&) const
{
    return ice_staticId();
}

const ::std::string&
RoboCompAprilTagsServer::AprilTagsServer::ice_staticId()
{
    static const ::std::string typeId = "::RoboCompAprilTagsServer::AprilTagsServer";
    return typeId;
}

/// \cond INTERNAL
bool
RoboCompAprilTagsServer::AprilTagsServer::_iceD_getAprilTags(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    auto istr = inS.startReadParams();
    Image iceP_frame;
    double iceP_tagsize;
    double iceP_mfx;
    double iceP_mfy;
    istr->readAll(iceP_frame, iceP_tagsize, iceP_mfx, iceP_mfy);
    inS.endReadParams();
    tagsList ret = this->getAprilTags(::std::move(iceP_frame), iceP_tagsize, iceP_mfx, iceP_mfy, current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}
/// \endcond

/// \cond INTERNAL
bool
RoboCompAprilTagsServer::AprilTagsServer::_iceDispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair<const ::std::string*, const ::std::string*> r = ::std::equal_range(iceC_RoboCompAprilTagsServer_AprilTagsServer_ops, iceC_RoboCompAprilTagsServer_AprilTagsServer_ops + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - iceC_RoboCompAprilTagsServer_AprilTagsServer_ops)
    {
        case 0:
        {
            return _iceD_getAprilTags(in, current);
        }
        case 1:
        {
            return _iceD_ice_id(in, current);
        }
        case 2:
        {
            return _iceD_ice_ids(in, current);
        }
        case 3:
        {
            return _iceD_ice_isA(in, current);
        }
        case 4:
        {
            return _iceD_ice_ping(in, current);
        }
        default:
        {
            assert(false);
            throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
        }
    }
}
/// \endcond

/// \cond INTERNAL
void
RoboCompAprilTagsServer::AprilTagsServerPrx::_iceI_getAprilTags(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<::RoboCompAprilTagsServer::tagsList>>& outAsync, const Image& iceP_frame, double iceP_tagsize, double iceP_mfx, double iceP_mfy, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name);
    outAsync->invoke(iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        [&](::Ice::OutputStream* ostr)
        {
            ostr->writeAll(iceP_frame, iceP_tagsize, iceP_mfx, iceP_mfy);
        },
        nullptr);
}
/// \endcond

/// \cond INTERNAL
::std::shared_ptr<::Ice::ObjectPrx>
RoboCompAprilTagsServer::AprilTagsServerPrx::_newInstance() const
{
    return ::IceInternal::createProxy<AprilTagsServerPrx>();
}
/// \endcond

const ::std::string&
RoboCompAprilTagsServer::AprilTagsServerPrx::ice_staticId()
{
    return AprilTagsServer::ice_staticId();
}

namespace Ice
{
}

#else // C++98 mapping

namespace
{

const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name = "getAprilTags";

}

/// \cond INTERNAL
::IceProxy::Ice::Object* ::IceProxy::RoboCompAprilTagsServer::upCast(AprilTagsServer* p) { return p; }

void
::IceProxy::RoboCompAprilTagsServer::_readProxy(::Ice::InputStream* istr, ::IceInternal::ProxyHandle< AprilTagsServer>& v)
{
    ::Ice::ObjectPrx proxy;
    istr->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new AprilTagsServer;
        v->_copyFrom(proxy);
    }
}
/// \endcond

::Ice::AsyncResultPtr
IceProxy::RoboCompAprilTagsServer::AprilTagsServer::_iceI_begin_getAprilTags(const ::RoboCompAprilTagsServer::Image& iceP_frame, ::Ice::Double iceP_tagsize, ::Ice::Double iceP_mfx, ::Ice::Double iceP_mfy, const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name, ::Ice::Normal, context);
        ::Ice::OutputStream* ostr = result->startWriteParams(::Ice::DefaultFormat);
        ostr->write(iceP_frame);
        ostr->write(iceP_tagsize);
        ostr->write(iceP_mfx);
        ostr->write(iceP_mfy);
        result->endWriteParams();
        result->invoke(iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

::RoboCompAprilTagsServer::tagsList
IceProxy::RoboCompAprilTagsServer::AprilTagsServer::end_getAprilTags(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAprilTagsServer_AprilTagsServer_getAprilTags_name);
    ::RoboCompAprilTagsServer::tagsList ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

/// \cond INTERNAL
::IceProxy::Ice::Object*
IceProxy::RoboCompAprilTagsServer::AprilTagsServer::_newInstance() const
{
    return new AprilTagsServer;
}
/// \endcond

const ::std::string&
IceProxy::RoboCompAprilTagsServer::AprilTagsServer::ice_staticId()
{
    return ::RoboCompAprilTagsServer::AprilTagsServer::ice_staticId();
}

RoboCompAprilTagsServer::AprilTagsServer::~AprilTagsServer()
{
}

/// \cond INTERNAL
::Ice::Object* RoboCompAprilTagsServer::upCast(AprilTagsServer* p) { return p; }

/// \endcond

namespace
{
const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[2] =
{
    "::Ice::Object",
    "::RoboCompAprilTagsServer::AprilTagsServer"
};

}

bool
RoboCompAprilTagsServer::AprilTagsServer::ice_isA(const ::std::string& s, const ::Ice::Current&) const
{
    return ::std::binary_search(iceC_RoboCompAprilTagsServer_AprilTagsServer_ids, iceC_RoboCompAprilTagsServer_AprilTagsServer_ids + 2, s);
}

::std::vector< ::std::string>
RoboCompAprilTagsServer::AprilTagsServer::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[0], &iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[2]);
}

const ::std::string&
RoboCompAprilTagsServer::AprilTagsServer::ice_id(const ::Ice::Current&) const
{
    return ice_staticId();
}

const ::std::string&
RoboCompAprilTagsServer::AprilTagsServer::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::RoboCompAprilTagsServer::AprilTagsServer";
    return typeId;
#else
    return iceC_RoboCompAprilTagsServer_AprilTagsServer_ids[1];
#endif
}

/// \cond INTERNAL
bool
RoboCompAprilTagsServer::AprilTagsServer::_iceD_getAprilTags(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    ::Ice::InputStream* istr = inS.startReadParams();
    Image iceP_frame;
    ::Ice::Double iceP_tagsize;
    ::Ice::Double iceP_mfx;
    ::Ice::Double iceP_mfy;
    istr->read(iceP_frame);
    istr->read(iceP_tagsize);
    istr->read(iceP_mfx);
    istr->read(iceP_mfy);
    inS.endReadParams();
    tagsList ret = this->getAprilTags(iceP_frame, iceP_tagsize, iceP_mfx, iceP_mfy, current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}
/// \endcond

namespace
{
const ::std::string iceC_RoboCompAprilTagsServer_AprilTagsServer_all[] =
{
    "getAprilTags",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

}

/// \cond INTERNAL
bool
RoboCompAprilTagsServer::AprilTagsServer::_iceDispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair<const ::std::string*, const ::std::string*> r = ::std::equal_range(iceC_RoboCompAprilTagsServer_AprilTagsServer_all, iceC_RoboCompAprilTagsServer_AprilTagsServer_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - iceC_RoboCompAprilTagsServer_AprilTagsServer_all)
    {
        case 0:
        {
            return _iceD_getAprilTags(in, current);
        }
        case 1:
        {
            return _iceD_ice_id(in, current);
        }
        case 2:
        {
            return _iceD_ice_ids(in, current);
        }
        case 3:
        {
            return _iceD_ice_isA(in, current);
        }
        case 4:
        {
            return _iceD_ice_ping(in, current);
        }
        default:
        {
            assert(false);
            throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
        }
    }
}
/// \endcond

/// \cond STREAM
void
RoboCompAprilTagsServer::AprilTagsServer::_iceWriteImpl(::Ice::OutputStream* ostr) const
{
    ostr->startSlice(ice_staticId(), -1, true);
    ::Ice::StreamWriter< AprilTagsServer, ::Ice::OutputStream>::write(ostr, *this);
    ostr->endSlice();
}

void
RoboCompAprilTagsServer::AprilTagsServer::_iceReadImpl(::Ice::InputStream* istr)
{
    istr->startSlice();
    ::Ice::StreamReader< AprilTagsServer, ::Ice::InputStream>::read(istr, *this);
    istr->endSlice();
}
/// \endcond

/// \cond INTERNAL
void
RoboCompAprilTagsServer::_icePatchObjectPtr(AprilTagsServerPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = AprilTagsServerPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(AprilTagsServer::ice_staticId(), v);
    }
}
/// \endcond

namespace Ice
{
}

#endif
