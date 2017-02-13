#include "BillingInterface.h"

#include "UniBilling.h"

BillingInterface* BillingImpl::m_pInstance = nullptr;

BillingInterface* BillingImpl::getInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new UniBilling();
	}

	return m_pInstance;
}

bool BillingImpl::isMusicEnabled()
{
	return m_pInstance->isMusicEnabled();
}

void BillingImpl::moreGame()
{
	m_pInstance->moreGame();
}

void BillingImpl::pay(const std::string& billingIndex, const std::string& code, const std::string& name, int price, const std::string& desc)
{
	m_pInstance->pay(billingIndex, code, name, price, desc);
}

bool BillingImpl::exit()
{
	return m_pInstance->exit();
}
